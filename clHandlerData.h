//----- CLASS HANDLERDATA_H ----------------------------------------------------
#ifndef clHandlerDataH
#define clHandlerDataH
//----- INCLUDES ---------------------------------------------------------------
#include "clTCLibraries.h"
#include <vector>
#include "clAbstractHandler.h"
#include "clLNTFuncs.h"
//----- CLASS HANDLERDATA ------------------------------------------------------
class clHandlerData: public clAbstractHandler
{
	public:
		 clHandlerData(clEPM_action_message *_args);
		~clHandlerData();
		int MainProcess();
  private:
		int GetPropArguments();
		int GetArguments();
		std::vector<AnsiString> ParserString(char *_cStr, char *_cDelim);
		int CheckArguments();
		int GetProblemReports();
		int SetState();
    int SetValue(tag_t _tObj, char *_cObjProp, char *_cNewObjProp);
	private:
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
