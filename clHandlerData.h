//----- CLASS HANDLERDATA_H ----------------------------------------------------
#ifndef clHandlerDataH
#define clHandlerDataH
//----- INCLUDES ---------------------------------------------------------------
#include "clTCLibraries.h"
#include "clAbstractHandler.h"
//----- CLASS HANDLERDATA ------------------------------------------------------
class clHandlerData: public clAbstractHandler
{
	public:
		 clHandlerData(clEPM_action_message *_args);
		~clHandlerData();
		int MainProcess();
		int GetPropArguments();
		int GetArguments();
		int CheckArguments();
		int GetProblemReports();
		int SetState();
	private:
		clTCLibraries *TC;
		clEPM_action_message *args;

		tag_t tCurrentTask;

		AnsiString strPRFilterMaturity;
		AnsiString strPRNewMaturity;
		AnsiString strPRNewClosure;
		AnsiString strTargetObjectReference;
		AnsiString strPRType;

		std::vector<AnsiString> vTargetObjectTypes;
		std::vector<tag_t> vecPRTags;

		bool Debug;
};
//------------------------------------------------------------------------------
#endif
