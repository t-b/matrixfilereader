/*
	The file operationsinterface_closeresultfile.cpp is part of the "MatrixFileReader XOP".
	It is licensed under the LGPLv3 with additional permissions,
	see License.txt	in the source folder for details.
*/

#include "header.h"

#include "operationstructs.h"
#include "operationsinterface.h"

#include "globaldata.h"

extern "C" int ExecuteCloseResultFile(CloseResultFileRuntimeParamsPtr p){
	BEGIN_OUTER_CATCH
	globDataPtr->initialize(p->calledFromMacro,p->calledFromFunction);

	if(!globDataPtr->resultFileOpen()){
		globDataPtr->setError(NO_FILE_OPEN);
		return 0;
	}
	globDataPtr->closeResultFile();

	globDataPtr->finalize();
	END_OUTER_CATCH
	return 0;
}