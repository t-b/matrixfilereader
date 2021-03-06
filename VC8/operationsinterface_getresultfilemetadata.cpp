/*
  The file operationsinterface_getresultfilemetadata.cpp is part of the "MatrixFileReader XOP".
  It is licensed under the LGPLv3 with additional permissions,
  see License.txt in the source folder for details.
*/

#include "stdafx.h"

#include "operationstructs.hpp"
#include "operationsinterface.hpp"
#include "utils_bricklet.hpp"
#include "globaldata.hpp"
#include "bricklet.hpp"
#include "utils_generic.hpp"

extern "C" int ExecuteGetResultFileMetaData(GetResultFileMetaDataRuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH
  GlobalData::Instance().initializeWithoutReadSettings(p->calledFromMacro, p->calledFromFunction);

  int ret = SetOperationStrVar(S_waveNames, "");

  if(ret != 0)
  {
    GlobalData::Instance().setInternalError(ret);
    return 0;
  }

  // check of DEST flag which tells us that we should place all output in the supplied datafolder
  // and also read the variable settings from this folder
  DataFolderHandle destDataFolderHndl = NULL;
  if(p->DESTFlagEncountered)
  {
    if(!dataFolderExists(p->dfref))
    {
      GlobalData::Instance().setError(WRONG_PARAMETER, "dfref");
      return 0;
    }

    destDataFolderHndl = p->dfref;
  }
  else // no DEST flag given, so we take the current data folder as destination folder
  {
    ret = GetCurrentDataFolder(&destDataFolderHndl);

    if(ret != 0)
    {
      GlobalData::Instance().setInternalError(ret);
      return 0;
    }
  }

  // Here we check again for the config variables, this time in the destDataFolderHndl
  GlobalData::Instance().readSettings(destDataFolderHndl);

  if(!GlobalData::Instance().resultFileOpen())
  {
    GlobalData::Instance().setError(NO_FILE_OPEN);
    return 0;
  }

  Vernissage::Session *session = GlobalData::Instance().getVernissageSession();
  const int numberOfBricklets  = session->getBrickletCount();

  // check waveName parameter
  std::string waveName = resultMetaDefault;
  if(p->NFlagEncountered)
  {
    if(WMGetHandleSize(p->waveName) == 0L)
    {
      GlobalData::Instance().setError(WRONG_PARAMETER, "waveName");
      return 0;
    }
    else
    {
      convertHandleToString(p->waveName, waveName);
    }
  }

  ret = CheckName(NULL, WAVE_OBJECT, waveName.c_str());
  if(ret == WAVE_NAME_TOO_LONG || ret == BAD_CHAR_IN_WAVE_NAME)
  {
    GlobalData::Instance().setError(WRONG_PARAMETER, "waveName");
    return 0;
  }

  StringPairVector data;
  AddEntry(data, RESULT_DIR_PATH_KEY, convertEncoding(GlobalData::Instance().getDirPath()));
  AddEntry(data, RESULT_FILE_NAME_KEY, convertEncoding(GlobalData::Instance().getFileName()));
  AddEntry(data, "totalNumberOfBricklets", toString(numberOfBricklets));

  if(numberOfBricklets > 0)
  {
    Bricklet &bricklet       = GlobalData::Instance().getBricklet(numberOfBricklets);
    void *vernissageBricklet = bricklet.getBrickletPointer();

    // use the timestamp of the last bricklet as dateOfLastChange
    tm ctime                                                     = session->getCreationTimestamp(vernissageBricklet);
    const Vernissage::Session::BrickletMetaData brickletMetaData = session->getMetaData(vernissageBricklet);

    char buf[ARRAY_SIZE];
    sprintf(buf, "%02d/%02d/%04d %02d:%02d:%02d", ctime.tm_mon + 1, ctime.tm_mday, ctime.tm_year + 1900, ctime.tm_hour,
            ctime.tm_min, ctime.tm_sec);

    AddEntry(data, "dateOfLastChange", std::string(buf));
    AddEntry(data, "timeStampOfLastChange", mktime(&ctime));
    AddEntry(data, "BrickletMetaData.fileCreatorName", brickletMetaData.fileCreatorName);
    AddEntry(data, "BrickletMetaData.fileCreatorVersion", brickletMetaData.fileCreatorVersion);
    AddEntry(data, "BrickletMetaData.userName", brickletMetaData.userName);
    AddEntry(data, "BrickletMetaData.accountName", brickletMetaData.accountName);
  }
  else
  {
    AddEntry(data, "dateOfLastChange", "");
    AddEntry(data, "timeStampOfLastChange", "");
    AddEntry(data, "BrickletMetaData.fileCreatorName", "");
    AddEntry(data, "BrickletMetaData.fileCreatorVersion", "");
    AddEntry(data, "BrickletMetaData.userName", "");
    AddEntry(data, "BrickletMetaData.accountName", "");
  }

  // brickletID=0 because we are handling resultfile metadata
  std::string waveNameList;
  ret = createAndFillTextWave(destDataFolderHndl, data, destDataFolderHndl, waveName.c_str(), 0, waveNameList);

  if(ret == WAVE_EXIST)
  {
    GlobalData::Instance().setError(ret, waveName);
    return 0;
  }
  else if(ret != 0)
  {
    GlobalData::Instance().setInternalError(ret);
    return 0;
  }

  ret = SetOperationStrVar(S_waveNames, waveNameList.c_str());

  if(ret != 0)
  {
    GlobalData::Instance().setInternalError(ret);
    return 0;
  }

  GlobalData::Instance().finalizeWithFilledCache();
  END_OUTER_CATCH
  return 0;
}
