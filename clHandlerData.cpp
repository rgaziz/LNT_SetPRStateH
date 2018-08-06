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
		listModules = new TStringList();
		listModules->Add("TC");
		listModules->Add("EPM");
		listModules->Add("TCCORE");
		listModules->Add("BASE_UTILS");
	TC->LoadLibraries(listModules);

  // Get tag from current task
	current_task_tag = args->GetTask();
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
		TC->TC->ITK__convert_tag_to_uid(current_task_tag, &UID);
			std::cout << "-Input arguments:CURRENT TASK["<< UID << "]: "<< std::endl;
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
  retcode = 0;
	int args_count = args->GetNumberOfArguments();

	if(args_count != 0)
	{
		for(int i = 0; i < args_count; i++)
		{
			 AnsiString argument_text = args->GetNextArgument();

			 if (argument_text.Pos("Debug") > 0) Debug = true;

			 if (argument_text.Pos("PRFilterMaturity=") == 1)
			 {
					PRFilterMaturity = StringReplace(argument_text,"PRFilterMaturity=","",
																					 TReplaceFlags() << rfReplaceAll);
			 }

			 if (argument_text.Pos("PRNewMaturity=") == 1)
			 {
					PRNewMaturity = StringReplace(argument_text, "PRNewMaturity=", "",
																				TReplaceFlags() << rfReplaceAll);
			 }

			 if (argument_text.Pos("PRNewClosure=") == 1)
			 {
					PRNewClosure = StringReplace(argument_text, "PRNewClosure=", "",
																			 TReplaceFlags() << rfReplaceAll);
			 }
    }
	}
  return retcode;
}

//----- GET INPUT PARAMETRS ----------------------------------------------------
int clHandlerData::GetArguments()
{
	AnsiString preference_name = "LNT_SetPRStateH";
	AnsiString scope = "s";

	TStringList *inputParams = NULL;
	// Get prefernce to TStringList
	inputParams = TC->TC->GetPrefValues(preference_name, scope);

	if (inputParams->Count == 0) {
		std::cout << "--Preference LNT_SetPRStateH not found! " << std::endl;
		return 919093;
	}

	for(int i = 0; i < inputParams->Count; i++)
	{
		AnsiString value_string = inputParams->Strings[i];

		if(value_string.Pos("TargetObjectTypes=") == 1)
		{
			value_string = StringReplace(value_string,"TargetObjectTypes=","",
																	TReplaceFlags() << rfReplaceAll);

			vTargetObjectTypes = ParserString(value_string.c_str(), ";");
		}

		if(value_string.Pos("TargetObjectReference=") == 1)
		{
			TargetObjectReference = StringReplace(value_string,"TargetObjectReference=",
																						"", TReplaceFlags() << rfReplaceAll);
		}

		if(value_string.Pos("PRType=") == 1)
		{
			PRType = StringReplace(value_string,"PRType=", "",
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

	if(PRFilterMaturity.Length() > 0)
	{
		if (Debug) std::cout << "----PRFilterMaturity("
												 << PRFilterMaturity.c_str()
												 << "):OK" << std::endl;
	}
	else
	{
		std::cout << "--PRFilterMaturity:ERROR" << std::endl;
		return 919093;
	}

	if(PRNewMaturity.Length() > 0)
	{
		if (Debug) std::cout << "----PRNewMaturity(" << PRNewMaturity.c_str()
		                     << "):OK" << std::endl;
	}
	else
	{
		std::cout << "--PRNewMaturity:ERROR" << std::endl;
		return 919093;
  }

	if(PRNewClosure.Length() > 0)
	{
		if (Debug) std::cout << "----PRNewClosure(" << PRNewClosure.c_str()
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
			for(int i = 0; i < vTargetObjectTypes.size(); i++)
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

	if (TargetObjectReference.Length() > 0)
	{
		if (Debug) std::cout << "----TargetObjectReference("
												 << TargetObjectReference.c_str() << "):OK" << std::endl;
	}
	else
	{
		std::cout << "--TargetObjectReference:ERROR" << std::endl;
		return 919093;
	}

	if (PRType.Length() > 0)
	{
		if (Debug) std::cout << "----PRType(" << PRType.c_str()
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

	std::vector<tag_t> vResultTargets;
	tag_t *target_object_tags = NULL;
	int num_targets = 0;

	//----  ƒÀﬂ –¿«Ã€ÿÀ≈Õ»ﬂ ------------------------------------------------------
	// Get target objects
	retcode = TC->TCCORE->AOM_ask_value_tags(current_task_tag,
																					 "root_target_attachments",
																					 &num_targets,
																					 &target_object_tags);
	if(retcode != 0)
	{
    retcode = TC->EPM->EPM_ask_attachments(current_task_tag,
																					EPM_target_attachment,
																					&num_targets,
																					&target_object_tags);
		if (retcode != 0)
		{
			ITK_ERR(retcode,  "EPM_ask_attachments", NULL);
		}

		ITK_ERR(retcode, "AOM_ask_value_tags", NULL);
    return retcode;
	}

 //-----  ŒÕ≈÷ –¿«Ã€ÿÀ≈Õ»ﬂ -----------------------------------------------------

	if (num_targets != 0)
	{
		for (int i = 0; i < num_targets; i++)
		{
			AnsiString object_type;
			object_type = TC->TCCORE->GetObjectTypeS(target_object_tags[i]);
			for (int j = 0; j < vTargetObjectTypes.size(); j++)
			{
				if (object_type.Trim() == vTargetObjectTypes[j].Trim())
						vResultTargets.push_back(target_object_tags[i]);
      }
		}
	}
	else
	{
		std::cout << "--Target objects not found!" << std::endl;
    return 919093;
  }

	TC->BASE_UTILS->MEM_free(target_object_tags);

	// check result
	if (vResultTargets.size() == 0)
	{
    for (int i = 0; i < vTargetObjectTypes.size(); i++)
		{
			std::cout << "--Target objects by type " << vTargetObjectTypes[i].c_str()
								<< " not found!" << std::endl;
		}
		return 919093;
	}

	vTargetObjectTypes.clear();

 for (int i = 0; i < vResultTargets.size(); i++)
 {
		tag_t *result_tags = NULL;
		int num_tags = 0;

		retcode = TC->TCCORE->AOM_ask_value_tags(vResultTargets[i],
																						 TargetObjectReference.c_str(),
																						 &num_tags,
                                             &result_tags);
		if (retcode != 0)
		{
			ITK_ERR(retcode, "AOM_ask_value_tags", NULL);
			return retcode;
		}

    for(int j = 0; j < num_tags; j++)
		{
			AnsiString obj_type = TC->TCCORE->GetObjectTypeS(result_tags[j]);
			if (obj_type == PRType)
			{
				vPRTags.push_back(result_tags[j]);
			}
		}

		TC->BASE_UTILS->MEM_free(result_tags);
 }

	vResultTargets.clear();

	//Check PRTags
	if (vPRTags.size() == 0)
	{
		std::cout << "--Problem Reports not found!" << std::endl;
	}
	else
	{
	 if (Debug)
	 {
			for(int i = 0; i < vPRTags.size(); i++)
			{
				char *UID = NULL;
					TC->TC->ITK__convert_tag_to_uid(vPRTags[i], &UID);
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

	if (vPRTags.size() != 0)
	for(int i = 0; i < vPRTags.size(); i++)
	{
		char *value = NULL;
		retcode = TC->TCCORE->AOM_ask_value_string(vPRTags[i], "CMMaturity", &value);
		if(retcode != 0)
		{
			ITK_ERR(retcode, "AOM_ask_value_string", NULL);
      return retcode;
		}

		AnsiString CMMaturity = value;

		// Set new values
	 if (CMMaturity == PRFilterMaturity)
	 {
			retcode = TC->TCCORE->AOM_lock(vPRTags[i]);
			if (retcode != 0)
			{
				ITK_ERR(retcode, "AOM_lock", NULL);
        return retcode;
      }

			retcode = TC->TCCORE->AOM_set_value_string(vPRTags[i], "CMMaturity",
																								 PRNewMaturity.c_str());
			if(retcode != 0)
			{
				ITK_ERR(retcode, "AOM_set_value_string", NULL);
        return retcode;
			}
			else if (Debug)
			{
				std::cout << "----CMMaturity = " << PRNewMaturity.c_str()
				          << ":OK" << std::endl;
      }

			retcode = TC->TCCORE->AOM_set_value_string(vPRTags[i], "CMClosure",
																								 PRNewClosure.c_str());
			if(retcode != 0)
			{
				ITK_ERR(retcode, "AOM_set_value_string", NULL);
        return retcode;
			}
			else if (Debug)
			{
				std::cout << "----CMClosure = " << PRNewClosure.c_str()
				          << ":OK" << std::endl;
			}

      // Save changes
			retcode = TC->TCCORE->AOM_save(vPRTags[i]);
			if (retcode != 0)
			{
				ITK_ERR(retcode, "AOM_save", NULL);
        return retcode;
      }

			retcode = TC->TCCORE->AOM_unlock(vPRTags[i]);
			if (retcode != 0)
			{
				ITK_ERR(retcode, "AOM_lock", NULL);
				return retcode;
			}
	 }

		TC->BASE_UTILS->MEM_free(value);
	}

	vPRTags.clear();

  if (Debug) std::cout << "-SetState:END" << std::endl;
	return retcode;
}

//----- DESTRUCTOR -------------------------------------------------------------
clHandlerData::~clHandlerData()
{
  Debug = false;
	vTargetObjectTypes.clear();
	vPRTags.clear();
	listModules->Clear();
	delete listModules;
  delete TC;
}
//------------------------------------------------------------------------------
#pragma package(smart_init)
//------------------------------------------------------------------------------
