/*
  The file operationsinterface_getxoperrormessage.cpp is part of the "MatrixFileReader XOP".
  It is licensed under the LGPLv3 with additional permissions,
  see License.txt in the source folder for details.
*/

#include "stdafx.h"

#include "operationstructs.hpp"
#include "operationsinterface.hpp"
#include "globaldata.hpp"

extern "C" int ExecuteGetXOPErrorMessage(GetXOPErrorMessageRuntimeParamsPtr p)
{
  std::string errorMessage;
  // return requested error message
  if(p->errorCodeEncountered && p->errorCodeParamsSet[0])
  {
    errorMessage = GlobalData::Instance().getErrorMessage(int(p->errorCode));
  }
  else // get last error message
  {
    errorMessage = GlobalData::Instance().getLastErrorMessage();
  }

  HISTPRINT(errorMessage.c_str());
  return 0;
}
