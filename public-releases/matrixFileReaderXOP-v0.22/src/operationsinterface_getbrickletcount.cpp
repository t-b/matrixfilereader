/*
  The file operationsinterface_getbrickletcount.cpp is part of the "MatrixFileReader XOP".
  It is licensed under the LGPLv3 with additional permissions,
  see License.txt in the source folder for details.
*/

#include "stdafx.h"

#include "operationstructs.hpp"
#include "operationsinterface.hpp"
#include "globaldata.hpp"
#include "utils_generic.hpp"

extern "C" int ExecuteGetBrickletCount(GetBrickletCountRuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH
  GlobalData::Instance().initialize(p->calledFromMacro, p->calledFromFunction);

  if (!GlobalData::Instance().resultFileOpen())
  {
    GlobalData::Instance().setError(NO_FILE_OPEN);
    return 0;
  }

  Vernissage::Session* session = GlobalData::Instance().getVernissageSession();
  ASSERT_RETURN_ZERO(session);

  int ret = SetOperationNumVar(V_count, session->getBrickletCount());

  if (ret != 0)
  {
    GlobalData::Instance().setInternalError(ret);
    return 0;
  }

  GlobalData::Instance().finalize();
  END_OUTER_CATCH
  return 0;
}
