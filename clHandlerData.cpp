//----- CLASS HANDLERDATA_CPP --------------------------------------------------
#pragma hdrstop
//----- INCLUDES ---------------------------------------------------------------
#include "clHandlerData.h"
// ----- CONSTRUCTOR -----------------------------------------------------------
clHandlerData::clHandlerData(clEPM_action_message *_args):clAbstractHandler(NULL)
{
	args = _args;
	Debug = false;

	TC = new clTCLibraries(NULL);
		AnsiString	listModules[] = {"TC", "EPM", "TCCORE", "BASE_UTILS"};
	TC->LoadLibraries(4, listModules);

  // Get tag from current task
	tCurrentTask = args->GetTask();
}

//----- MAIN PROCESS -----------------------------------------------------------
int clHandlerData::MainProcess()
{
	std::cout << "-LNT_SetPRStateH. Version - "<< GetHandlerVersion()<< std::endl;

	// Get Handler property arguments
	retcode = GetPropArguments();
		if (retcode != 0) return retcode;

	if (Debug) std::cout << "===== START =====" << std::endl;

	// UID from current TASK
	char *UID = NULL;
		TC->TC->ITK__convert_tag_to_uid(tCurrentTask, &UID);
			std::cout << "-Input arguments:CURRENT TASK["<< UID << "]:"<< std::endl;
    TC->BASE_UTILS->MEM_free(UID);

	// Get preference arguments
	retcode = GetArguments();
		if (retcode != 0) return retcode;

	// Check input arguments
	retcode = CheckArguments();
		if (retcode != 0) return retcode;

	// Get PR's tags
	retcode = GetProblemReports();
		if (retcode != 0) return retcode;

  // Set state to problem report
	retcode = SetState();
		if (retcode != 0) return retcode;

	if (Debug) std::cout << "===== END =====" << std::endl;
		return retcode;
}

//----- GET PROPERTY ARGUMENTS -------------------------------------------------
int clHandlerData::GetPropArguments()
{
	int iArgsCount = args->GetNumberOfArguments();

	if (iArgsCount != 0)
	{
		for (int i = 0; i < iArgsCount; i++)
		{
			 AnsiString strArgumentText = args->GetNextArgument();

			 if (strArgumentText.Pos("Debug") > 0) Debug = true;

			 if (strArgumentText.Pos("PRFilterMaturity=") == 1)
			 {
					strPRFilterMaturity = StringReplace(strArgumentText,"PRFilterMaturity=","",
																					 TReplaceFlags() << rfReplaceAll);
			 }

			 if (strArgumentText.Pos("PRNewMaturity=") == 1)
			 {
					strPRNewMaturity = StringReplace(strArgumentText, "PRNewMaturity=", "",
																				TReplaceFlags() << rfReplaceAll);
			 }

			 if (strArgumentText.Pos("PRNewClosure=") == 1)
			 {
					strPRNewClosure = StringReplace(strArgumentText, "PRNewClosure=", "",
																			 TReplaceFlags() << rfReplaceAll);
			 }
    }
	}
  return retcode;
}

//----- GET INPUT PARAMETRS ----------------------------------------------------
int clHandlerData::GetArguments()
{
	AnsiString strPreferenceName = "LNT_SetPRStateH";
	AnsiString strScope = "s";

	TStringList *inputParams = NULL;
	// Get prefernce to TStringList
	inputParams = TC->TC->GetPrefValues(strPreferenceName, strScope);

	if (inputParams->Count == 0)
	{
		std::cout << "--Preference LNT_SetPRStateH not found! " << std::endl;
		return 919093;
	}

	for (int i = 0; i < inputParams->Count; i++)
	{
		AnsiString strTemp = inputParams->Strings[i];

		if (strTemp.Pos("TargetObjectTypes=") == 1)
		{
			strTemp = StringReplace(strTemp,"TargetObjectTypes=","",
																	TReplaceFlags() << rfReplaceAll);

			vTargetObjectTypes = ParserString(strTemp.c_str(), ";");
		}

		if (strTemp.Pos("TargetObjectReference=") == 1)
		{
			strTargetObjectReference = StringReplace(strTemp,"TargetObjectReference=",
																						"", TReplaceFlags() << rfReplaceAll);
		}

		if (strTemp.Pos("PRType=") == 1)
		{
			strPRType = StringReplace(strTemp,"PRType=", "",
														TReplaceFlags() << rfReplaceAll);
    }
  }

	return retcode;
}

//----- CHECK ARGUMENTS --------------------------------------------------------
int clHandlerData::CheckArguments()
{
	if (Debug)
	{
		std::cout << "-CheckArguments:START" << std::endl;
		std::cout << "----Debug:OK" << std::endl;
	}

	if (strPRFilterMaturity.Length() > 0)
	{
		if (Debug) std::cout << "----PRFilterMaturity("
												 << strPRFilterMaturity.c_str()
												 << "):OK" << std::endl;
	}
	else
	{
		std::cout << "--PRFilterMaturity:ERROR" << std::endl;
		return 919093;
	}

	if(strPRNewMaturity.Length() > 0)
	{
		if (Debug) std::cout << "----PRNewMaturity(" << strPRNewMaturity.c_str()
		                     << "):OK" << std::endl;
	}
	else
	{
		std::cout << "--PRNewMaturity:ERROR" << std::endl;
		return 919093;
  }

	if(strPRNewClosure.Length() > 0)
	{
		if (Debug) std::cout << "----PRNewClosure(" << strPRNewClosure.c_str()
												 << "):OK" << std::endl;
	}
	else
	{
		std::cout << "--PRNewClosure:ERROR" << std::endl;
		return 919093;
	}

	if (vTargetObjectTypes.size() > 0)
	{
		if (Debug)
		{
			for (int i = 0; i < vTargetObjectTypes.size(); i++)
			{
				std::cout << "----TargetObjectTypes(" << vTargetObjectTypes[i]
									<< "):OK" << std::endl;
			}
		}
	}
	else
	{
		std::cout << "--TargetObjectTypes:ERROR" << std::endl;
		return 919093;
	}

	if (strTargetObjectReference.Length() > 0)
	{
		if (Debug) std::cout << "----TargetObjectReference("
												 << strTargetObjectReference.c_str() << "):OK" << std::endl;
	}
	else
	{
		std::cout << "--TargetObjectReference:ERROR" << std::endl;
		return 919093;
	}

	if (strPRType.Length() > 0)
	{
		if (Debug) std::cout << "----PRType(" << strPRType.c_str()
												 << "):OK" << std::endl;
	}
	else
	{
		std::cout << "--PRType:ERROR" << std::endl;
    return 919093;
  }

	if (Debug) std::cout << "-CheckArguments:END" << std::endl;
	return retcode;
}

//----- GET PROBLEM REPORTS ----------------------------------------------------
int clHandlerData::GetProblemReports()
{
	if (Debug) std::cout << "-GetProblemReports:START" << std::endl;

	retcode  = 0;

	std::vector<tag_t> vecResultTargets;
	tag_t *tTaregetObjects = NULL;
	int iTargetCount = 0;

	// Get target objects
	retcode = TC->EPM->EPM_ask_attachments(tCurrentTask,
																					EPM_target_attachment,
																					&iTargetCount,
																					&tTaregetObjects);
	if (retcode != 0)
	{
		ITK_ERR(retcode,  "EPM_ask_attachments", NULL);
		return retcode;
	}

	if (iTargetCount != 0)
	{
		for (int i = 0; i < iTargetCount; i++)
		{
			AnsiString strObjectType = TC->TCCORE->GetObjectTypeS(tTaregetObjects[i]);
			for (int j = 0; j < vTargetObjectTypes.size(); j++)
			{
				if (strObjectType.Trim() == vTargetObjectTypes[j].Trim())
						vecResultTargets.push_back(tTaregetObjects[i]);
      }
		}
	}
	else
	{
		std::cout << "--Target objects not found!" << std::endl;
    return 919093;
  }

	TC->BASE_UTILS->MEM_free(tTaregetObjects);

	// check result
	if (vecResultTargets.size() == 0)
	{
    for (int i = 0; i < vTargetObjectTypes.size(); i++)
		{
			std::cout << "--Target objects by type " << vTargetObjectTypes[i].c_str()
								<< " not found!" << std::endl;
		}
		return 919093;
	}

	vTargetObjectTypes.clear();

 for (int i = 0; i < vecResultTargets.size(); i++)
 {
		tag_t *tResults = NULL;
		int iTagsCount = 0;

		retcode = TC->TCCORE->AOM_ask_value_tags(vecResultTargets[i],
																						 strTargetObjectReference.c_str(),
																						 &iTagsCount,
																						 &tResults);
		if (retcode != 0)
		{
			ITK_ERR(retcode, "AOM_ask_value_tags", NULL);
			return retcode;
		}

		for (int j = 0; j < iTagsCount; j++)
		{
			AnsiString obj_type = TC->TCCORE->GetObjectTypeS(tResults[j]);
			if (obj_type == strPRType)
			{
				vecPRTags.push_back(tResults[j]);
			}
		}

		TC->BASE_UTILS->MEM_free(tResults);
 }

	vecResultTargets.clear();

	//Check PRTags
	if (vecPRTags.size() == 0)
	{
		std::cout << "--Problem Reports not found!" << std::endl;
	}
	else
	{
	 if (Debug)
	 {
			for (int i = 0; i < vecPRTags.size(); i++)
			{
				char *UID = NULL;
					TC->TC->ITK__convert_tag_to_uid(vecPRTags[i], &UID);
						std::cout << "----Problem Report[" << UID << "]:OK" << std::endl;
					TC->BASE_UTILS->MEM_free(UID);
			}
	 }
	}

  if (Debug) std::cout << "-GetProblemReports:END" << std::endl;
	return retcode;
}

//----- SET STATE --------------------------------------------------------------
int clHandlerData::SetState()
{
	if (Debug) std::cout << "-SetState:START" << std::endl;

	if (vecPRTags.size() != 0)
	for (int i = 0; i < vecPRTags.size(); i++)
	{
		char *cValue = NULL;
		retcode = TC->TCCORE->AOM_ask_value_string(vecPRTags[i], "CMMaturity", &cValue);
		if(retcode != 0)
		{
			ITK_ERR(retcode, "AOM_ask_value_string", NULL);
      return retcode;
		}

		AnsiString CMMaturity = cValue;

		// Set new values
	 if (CMMaturity == strPRFilterMaturity)
	 {
			retcode = TC->TCCORE->AOM_lock(vecPRTags[i]);
			if (retcode != 0)
			{
				ITK_ERR(retcode, "AOM_lock", NULL);
        return retcode;
      }

			retcode = TC->TCCORE->AOM_set_value_string(vecPRTags[i], "CMMaturity",
																								 strPRNewMaturity.c_str());
			if(retcode != 0)
			{
				ITK_ERR(retcode, "AOM_set_value_string", NULL);
        return retcode;
			}
			else if (Debug)
			{
				std::cout << "----CMMaturity = " << strPRNewMaturity.c_str()
				          << ":OK" << std::endl;
      }

			retcode = TC->TCCORE->AOM_set_value_string(vecPRTags[i], "CMClosure",
																								 strPRNewClosure.c_str());
			if(retcode != 0)
			{
				ITK_ERR(retcode, "AOM_set_value_string", NULL);
        return retcode;
			}
			else if (Debug)
			{
				std::cout << "----CMClosure = " << strPRNewClosure.c_str()
				          << ":OK" << std::endl;
			}

      // Save changes
			retcode = TC->TCCORE->AOM_save(vecPRTags[i]);
			if (retcode != 0)
			{
				ITK_ERR(retcode, "AOM_save", NULL);
        return retcode;
      }

			retcode = TC->TCCORE->AOM_unlock(vecPRTags[i]);
			if (retcode != 0)
			{
				ITK_ERR(retcode, "AOM_lock", NULL);
				return retcode;
			}
	 }
		TC->BASE_UTILS->MEM_free(cValue);
	}

	vecPRTags.clear();

  if (Debug) std::cout << "-SetState:END" << std::endl;
	return retcode;
}

//----- DESTRUCTOR -------------------------------------------------------------
clHandlerData::~clHandlerData()
{
  Debug = false;
	vTargetObjectTypes.clear();
	vecPRTags.clear();
  delete TC;
}
//------------------------------------------------------------------------------
#pragma package(smart_init)
//------------------------------------------------------------------------------
