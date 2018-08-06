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
		// Include ITK functions
		clTCLibraries *TC;
		TStringList *listModules;

		clEPM_action_message *args;

		tag_t current_task_tag;

		AnsiString PRFilterMaturity;
		AnsiString PRNewMaturity;
		AnsiString PRNewClosure;
		AnsiString TargetObjectReference;
		AnsiString PRType;

		std::vector<AnsiString> vTargetObjectTypes;
		std::vector<tag_t> vPRTags;

		bool Debug;
};
//------------------------------------------------------------------------------
#endif
