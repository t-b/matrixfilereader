/*
	The file operationsinterface.cpp is part of the "MatrixFileReader XOP".
	It is licensed under the LGPLv3 with additional permissions,
	see License.txt	in the source folder for details.
*/

#include "header.h"

#include "operationstructs.h"
#include "operationsinterface.h"

#include "utils_bricklet.h"
#include "globaldata.h"
#include "preferences.h"

static int RegisterGetResultFileMetaData(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the GetResultFileMetaDataRuntimeParams structure as well.
	cmdTemplate = "MFR_GetResultFileMetaData /N=string:waveName /DEST=dataFolderRef:dfref";
	runtimeNumVarList = V_flag;
	runtimeStrVarList = S_waveNames;
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(GetResultFileMetaDataRuntimeParams), (void*)ExecuteGetResultFileMetaData, 0);
}

static int RegisterGetMtrxFileReaderVersion(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the GetMtrxFileReaderVersionRuntimeParams structure as well.
	cmdTemplate = "MFR_GetVersion";
	runtimeNumVarList = V_XOPversion;
	runtimeStrVarList = "";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(GetVersionRuntimeParams), (void*)ExecuteGetVersion, 0);
}

static int RegisterGetVernissageVersion(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the GetVernissageVersionRuntimeParams structure as well.
	cmdTemplate = "MFR_GetVernissageVersion";
	runtimeNumVarList = V_DLLversion;
	runtimeStrVarList = "";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(GetVernissageVersionRuntimeParams), (void*)ExecuteGetVernissageVersion, 0);
}

static int RegisterGetXOPErrorMessage(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the GetXOPErrorMessageRuntimeParams structure as well.
	cmdTemplate = "MFR_GetXOPErrorMessage [number:errorCode]";
	runtimeNumVarList = "";
	runtimeStrVarList = "";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(GetXOPErrorMessageRuntimeParams), (void*)ExecuteGetXOPErrorMessage, 0);
}

static int RegisterOpenResultFile(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the OpenResultFileRuntimeParams structure as well.
	cmdTemplate = "MFR_OpenResultFile /K /P=name:pathName [string:fileNameOrPath]";
	runtimeNumVarList = V_flag;
	runtimeStrVarList = "";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(OpenResultFileRuntimeParams), (void*)ExecuteOpenResultFile, 0);
}

static int RegisterCloseResultFile(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the CloseResultFileRuntimeParams structure as well.
	cmdTemplate = "MFR_CloseResultFile";
	runtimeNumVarList = V_flag;
	runtimeStrVarList = "";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(CloseResultFileRuntimeParams), (void*)ExecuteCloseResultFile, 0);
}

static int RegisterGetBrickletCount(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the GetBrickletCountRuntimeParams structure as well.
	cmdTemplate = "MFR_GetBrickletCount";
	runtimeNumVarList = "V_flag;V_count";
	runtimeStrVarList = "";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(GetBrickletCountRuntimeParams), (void*)ExecuteGetBrickletCount, 0);
}

static int RegisterGetResultFileName(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the GetResultFileNameRuntimeParams structure as well.
	cmdTemplate = "MFR_GetResultFileName";
	runtimeNumVarList = V_flag;
	runtimeStrVarList = "S_fileName;S_dirPath";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(GetResultFileNameRuntimeParams), (void*)ExecuteGetResultFileName, 0);
}

static int RegisterGetBrickletData(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the MFR_GetBrickletDataRuntimeParams structure as well.
	cmdTemplate = "MFR_GetBrickletData /R=(number:startBrickletID[,number:endBrickletID]) /S=number:pixelSize /N=string:baseName /DEST=dataFolderRef:dfref";
	runtimeNumVarList = V_flag;
	runtimeStrVarList = S_waveNames;
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(GetBrickletDataRuntimeParams), (void*)ExecuteGetBrickletData, 0);
}


static int RegisterGetBrickletMetaData(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the GetBrickletMetaDataRuntimeParams structure as well.
	cmdTemplate = "MFR_GetBrickletMetaData /R=(number:startBrickletID[,number:endBrickletID]) /N=string:baseName /DEST=dataFolderRef:dfref";
	runtimeNumVarList = V_flag;
	runtimeStrVarList = S_waveNames;
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(GetBrickletMetaDataRuntimeParams), (void*)ExecuteGetBrickletMetaData, 0);
}

static int RegisterGetBrickletRawData(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the GetBrickletRawDataRuntimeParams structure as well.
	cmdTemplate = "MFR_GetBrickletRawData /R=(number:startBrickletID[,number:endBrickletID]) /N=string:baseName /DEST=dataFolderRef:dfref";
	runtimeNumVarList = V_flag;
	runtimeStrVarList = S_waveNames;
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(GetBrickletRawDataRuntimeParams), (void*)ExecuteGetBrickletRawData, 0);
}

static int RegisterGetReportTemplate(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the GetReportTemplateRuntimeParams structure as well.
	cmdTemplate = "MFR_GetReportTemplate";
	runtimeNumVarList = "";
	runtimeStrVarList = S_value;
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(GetReportTemplateRuntimeParams), (void*)ExecuteGetReportTemplate, 0);
}

static int RegisterCreateOverviewTable(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the CreateOverviewTableRuntimeParams structure as well.
	cmdTemplate = "MFR_CreateOverviewTable /N=string:waveName /KEYS=string:keyList /DEST=dataFolderRef:dfref";
	runtimeNumVarList = V_flag;
	runtimeStrVarList = S_waveNames;
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(CreateOverviewTableRuntimeParams), (void*)ExecuteCreateOverviewTable, 0);
}

static int RegisterCheckForNewBricklets(void){
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the CheckForNewBrickletsRuntimeParams structure as well.
	cmdTemplate = "MFR_CheckForNewBricklets";
	runtimeNumVarList = "V_flag;V_startBrickletID;V_endBrickletID";
	runtimeStrVarList = "";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(CheckForNewBrickletsRuntimeParams), (void*)ExecuteCheckForNewBricklets, 0);
}

/*
This is the entry point from the host application to the XOP for all messages after the
INIT message.
*/
extern "C" void XOPEntry(void){
	XOPIORecResult result = 0;

	try{
		switch (GetXOPMessage()) {
			case CLEANUP:
				saveXOPPreferences();
				// in case the user has forgotten to close the result file
				if(globDataPtr->resultFileOpen()){
					globDataPtr->closeResultFile();
				}
				// close the session and unload the DLL
				globDataPtr->closeSession();
				delete globDataPtr;
				globDataPtr = NULL;
			break;
		}
	}
	catch(...){
		XOPNotice("Unexpected exception in XOPEntry");
	}
	SetXOPResult(result);
}

/*	XOPMain(ioRecHandle)

This is the initial entry point at which the host application calls XOP.
The message sent by the host must be INIT.

*/

HOST_IMPORT int XOPMain(IORecHandle ioRecHandle){	
	XOPInit(ioRecHandle);							/* do standard XOP initialization */
	SetXOPEntry(XOPEntry);							/* set entry point for future calls */
	int errorCode;

	if (igorVersion < 620){
		SetXOPResult(REQUIRES_IGOR_620);
		return EXIT_FAILURE;
	}

	try{
		new GlobalData();
	}
	catch(...){
		SetXOPResult(BROKEN_XOP);
		return EXIT_FAILURE;
	}

	// load preferences from file
	loadXOPPreferences();

	if (errorCode = RegisterOpenResultFile()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterCloseResultFile()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterGetBrickletCount()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterGetResultFileName()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterGetVernissageVersion()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterGetMtrxFileReaderVersion()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterGetBrickletData()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterGetBrickletMetaData()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterGetBrickletRawData()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterGetReportTemplate()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterCreateOverviewTable()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterGetResultFileMetaData()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterCheckForNewBricklets()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}
	if (errorCode = RegisterGetXOPErrorMessage()) {
		SetXOPResult(errorCode);
		return EXIT_FAILURE;
	}

	SetXOPResult(0L);
	return EXIT_SUCCESS;
}
