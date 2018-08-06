//----- CLASS ACTIONHANDLER_H --------------------------------------------------
#ifndef ActionHandlerH
#define ActionHandlerH
//----- INCLUDES ---------------------------------------------------------------
#include "clHandlerData.h"
//----- FUNCTIONS --------------------------------------------------------------
extern "C" __export int __stdcall RegisterMethodsGlobal(char *retString);
extern "C" __declspec(dllexport) int LNT_SetPRStateH_register_callbacks();
int RegisterFunction(char *retString);
int LNT_SetPRStateH(EPM_action_message_t<TC_argument_t8> *msg8,
										EPM_action_message_t<TC_argument_t11> *msg11);
int LNT_SetPRStateH_v8(EPM_action_message_t<TC_argument_t8> msg);
int LNT_SetPRStateH_v11(EPM_action_message_t<TC_argument_t11> msg);
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
