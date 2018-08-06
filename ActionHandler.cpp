//----- CLASS ACTIONHANDLER_CPP ------------------------------------------------
#pragma hdrstop
//----- INCLUDES ---------------------------------------------------------------
#include "ActionHandler.h"

//----- REGISTER FUNCTION WITH TC_CUSTOMIZATION --------------------------------
 extern "C" __declspec(dllexport) int LNT_SetPRStateH_register_callbacks()
 {
	 retcode = RegisterFunction(NULL);
			if (retcode != 0) std::cout << "Error register function!" << std::endl;
	 return retcode;
 }

//----- REGISTER FUNCTION WITH LNT_LOADLIBRARY ---------------------------------
extern "C" __export int __stdcall RegisterMethodsGlobal(char *retString)
{
	retcode = RegisterFunction(retString);
			if (retcode != 0) std::cout << "Error register function!" << std::endl;
	return retcode;
}

//----- REGISTER ACTON HANDLER -------------------------------------------------
int RegisterFunction(char *retString)
{
	clTCLibraries *TC = new clTCLibraries(NULL);
		TStringList *listModules = new TStringList();
			listModules->Add("EPM");
	TC->LoadLibraries(listModules);

	int retcode = 0;
	retcode = TC->EPM->Register_action_handler("LNT_SetPRStateH",
																						 "LNT_SetPRStateH",
																							LNT_SetPRStateH_v8,
																							LNT_SetPRStateH_v11 );
	if(retcode != ITK_ok)
	{
		std::cout << " Error Register_action_handler - " << retcode << std::endl;
    return retcode;
	}

	if( retString != NULL)
	{
		if (ITK_ok == retcode) strcpy(retString, "OK");
		else strcpy(retString, AnsiString(retcode).c_str());
	}
	else
	{
		std::cout << "[LNT_SetPRStateH]:OK" << std::endl;
  }

	listModules->Clear();
	delete listModules;
	delete TC;

	return(retcode);
}

//----- REGISTER FUNCTIONS -----------------------------------------------------
int LNT_SetPRStateH(EPM_action_message_t<TC_argument_t8> *_msg8,
					EPM_action_message_t<TC_argument_t11> *_msg11)
{
	clHandlerData *hData = NULL;

	if(_msg8 != NULL)
	{
		hData = new clHandlerData(new clEPM_action_message(_msg8));
	}
	else if(_msg11 != NULL)
	{
		hData = new clHandlerData(new clEPM_action_message(_msg11));
	}
	else
	{
		std::cout << "Error register functions! " << std::endl;
		return 0;
	}

	// Main process from clHandlerData
	retcode = hData->MainProcess();
	if(retcode != 0)
	{
		return retcode;
	}

	delete hData;
	return(retcode);
}

//----- LNT_SetPRStateH_V8 -----------------------------------------------------
int LNT_SetPRStateH_v8(EPM_action_message_t<TC_argument_t8> msg)
{
	int retcode = LNT_SetPRStateH(&msg, NULL);
	return(retcode);
}

//----- LNT_SetPRStateH_V11 ----------------------------------------------------
int LNT_SetPRStateH_v11(EPM_action_message_t<TC_argument_t11> msg)
{
	int retcode = LNT_SetPRStateH(NULL, &msg);
	return(retcode);
}
//------------------------------------------------------------------------------
#pragma package(smart_init)
