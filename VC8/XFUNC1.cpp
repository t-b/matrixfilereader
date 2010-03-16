/*	XFUNC1.c -- illustrates Igor external functions.

*/

#include "XOPStandardHeaders.h"			// Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h
#include "XFUNC1.h"

#include "dataclass.h"

#include "globalvariables.h"
#include "utils.h"

#define DEBUG_LEVEL 1

#include "Vernissage.h"

#define ASSERT(A) { if(A == NULL){ XOPNotice("ASSERT: Pointer " #A " is NULL. You might want to drop the author a note :)\r"); return 0; } }

#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct checkForNewBrickletsParams{
	double  rememberCalls;
	double *endBrickletID;
	double *startBrickletID;
	double  result;	
};
typedef struct checkForNewBrickletsParams checkForNewBrickletsParams;
#pragma pack()

// variable checkForNewBricklets(variable *startBrickletID,variable *endBrickletID,variable rememberCalls)
static int checkForNewBricklets(checkForNewBrickletsParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}

#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct closeResultFileParams{
	double  result;	
};
typedef struct closeResultFileParams closeResultFileParams;
#pragma pack()

// variable closeResultFile()
static int closeResultFile(closeResultFileParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);

	pSession->eraseResultSets();
	pMyData->closeSession();

	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getAllBrickletDataParams{
	double  separateFolderForEachBricklet;
	Handle  baseName;
	Handle  dataFolderPath;
	double  result;	
};
typedef struct getAllBrickletDataParams getAllBrickletDataParams;
#pragma pack()

// variable getAllBrickletData(string dataFolderPath, string baseName, variable separateFolderForEachBricklet)
static int getAllBrickletData(getAllBrickletDataParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}


#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getAllBrickletMetaDataParams{
	double  separateFolderForEachBricklet;
	Handle  baseName;
	Handle  dataFolderPath;
	double  result;	
};
typedef struct getAllBrickletMetaDataParams getAllBrickletMetaDataParams;
#pragma pack()

// variable getAllBrickletMetaData(string dataFolderPath, string baseName, variable separateFolderForEachBricklet)
static int getAllBrickletMetaData(getAllBrickletMetaDataParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getBrickletDataParams{
	double  brickletID;
	double  separateFolderForEachBricklet;
	Handle  baseName;
	Handle  dataFolderPath;
	double  result;	
};
typedef struct getBrickletDataParams getBrickletDataParams;
#pragma pack()

// variable getBrickletData(string dataFolderPath, string baseName, variable separateFolderForEachBricklet, variable brickletID)
static int getBrickletData(getBrickletDataParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getBrickletDimensionParams{
	double *brickletDimension;
	double  brickletID;
	double  result;	
};
typedef struct getBrickletDimensionParams getBrickletDimensionParams;
#pragma pack()

// variable getBrickletDimension(variable brickletID, variable *brickletDimension)
static int getBrickletDimension(getBrickletDimensionParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getBrickletMetaDataParams{
	waveHndl  metaData;
	double  brickletID;
	double  result;	
};
typedef struct getBrickletMetaDataParams getBrickletMetaDataParams;
#pragma pack()

// variable getBrickletMetaData(variable brickletID, TEXTWAVE metaData)
static int getBrickletMetaData(getBrickletMetaDataParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getBrickletRawDataParams{
	waveHndl  data;
	double  brickletID;
	double  result;	
};
typedef struct getBrickletRawDataParams getBrickletRawDataParams;
#pragma pack()

// variable getBrickletRawData(variable brickletID, FP64WAVE data)
static int getBrickletRawData(getBrickletRawDataParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getBrickletViewTypeCodeParams{
	waveHndl  allViewTypeCodes;
	double  brickletID;
	double  result;	
};
typedef struct getBrickletViewTypeCodeParams getBrickletViewTypeCodeParams;
#pragma pack()

// variable getBrickletViewTypeCode(variable brickletID, FP64WAVE allViewTypeCodes)
static int getBrickletViewTypeCode(getBrickletViewTypeCodeParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getErrorMessageParams{
	double  errorCode;
	Handle  result;	
};
typedef struct getErrorMessageParams getErrorMessageParams;
#pragma pack()

// string getHumanReadableStringFromErrorCode(variable errorCode)
static int getErrorMessage(getErrorMessageParams *p){

	//p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		//p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	//p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getNumberOfBrickletsParams{
	double *totalNumberOfBricklets;
	double  result;	
};
typedef struct getNumberOfBrickletsParams getNumberOfBrickletsParams;
#pragma pack()

// variable getNumberOfBricklets(variable *totalNumberOfBricklets)
static int getNumberOfBricklets(getNumberOfBrickletsParams *p){

	p->result = UNKOWN_ERROR;
	*p->totalNumberOfBricklets = -1;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);

	*p->totalNumberOfBricklets = pSession->getBrickletCount();

	if(*p->totalNumberOfBricklets == 0){
		p->result = EMPTY_RESULTFILE;
	}
	else{
		p->result = SUCCESS;
	}

	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getRangeBrickletDataParams{
	double  endBrickletID;
	double  startBrickletID;
	double  separateFolderForEachBricklet;
	Handle  baseName;
	Handle  dataFolderPath;
	double  result;	
};
typedef struct getRangeBrickletDataParams getRangeBrickletDataParams;
#pragma pack()

// variable getRangeBrickletData(string dataFolderPath, string baseName, variable separateFolderForEachBricklet, variable startBrickletID, variable endBrickletID)
static int getRangeBrickletData(getRangeBrickletDataParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getRangeBrickletMetaDataParams{
	double  endBrickletID;
	double  startBrickletID;
	double  separateFolderForEachBricklet;
	Handle  baseName;
	Handle  dataFolderPath;
	double  result;	
};
typedef struct getRangeBrickletMetaDataParams getRangeBrickletMetaDataParams;
#pragma pack()

// variable getRangeBrickletMetaData(string dataFolderPath,string baseName,variable separateFolderForEachBricklet, variable startBrickletID, variable endBrickletID)
static int getRangeBrickletMetaData(getRangeBrickletMetaDataParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getResultFileMetaDataParams{
	waveHndl  resultFileMetaData;
	double  result;	
};
typedef struct getResultFileMetaDataParams getResultFileMetaDataParams;
#pragma pack()

// variable getResultFileMetaData(TEXTWAVE resultFileMetaData)
static int getResultFileMetaData(getResultFileMetaDataParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getResultFileNameParams{
	Handle *filename;
	double  result;	
};
typedef struct getResultFileNameParams getResultFileNameParams;
#pragma pack()

// variable getResultFileName(string *filename)
static int getResultFileName(getResultFileNameParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getResultFilePathParams{
	Handle *absoluteFilePath;
	double  result;	
};
typedef struct getResultFilePathParams getResultFilePathParams;
#pragma pack()

// variable getResultFilePath(string *absoluteFilePath)
static int getResultFilePath(getResultFilePathParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getVernissageVersionParams{
	double *vernissageVersion;
	double  result;	
};
typedef struct getVernissageVersionParams getVernissageVersionParams;
#pragma pack()

// variable getVernissageVersion(variable *vernissageVersion)
static int getVernissageVersion(getVernissageVersionParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}




#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct getXOPVersionParams{
	double *xopVersion;
	double  result;	
};
typedef struct getXOPVersionParams getXOPVersionParams;
#pragma pack()

// variable getXOPVersion(variable *xopVersion)
static int getXOPVersion(getXOPVersionParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(!pMyData->resultFileOpen()){
		p->result = NO_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);



	p->result = SUCCESS;
	return 0;
}



#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct openResultFileParams{
	Handle  fileName;
	Handle  absoluteFilePath;
	double  result;	
};
typedef struct openResultFileParams openResultFileParams;
#pragma pack()

// variable openResultFile(string absoluteFilePath, string fileName)
static int openResultFile(openResultFileParams *p){

	p->result = UNKOWN_ERROR;

	ASSERT(pMyData);
	if(pMyData->resultFileOpen()){
		p->result = ALREADY_FILE_OPEN;
		return 0;
	}

	Vernissage::Session *pSession = pMyData->getSession();
	ASSERT(pSession);

	char filePath[MAX_PATH_LEN+1];
	char fileName[MAX_PATH_LEN+1];
	std::wstring wstringFilePath,wstringFileName;

	int returnValueInt = 0;
	
	returnValueInt = GetCStringFromHandle(p->absoluteFilePath,filePath,MAX_PATH_LEN);
	if( returnValueInt != 0 ){
		return returnValueInt;
	}

	returnValueInt = GetCStringFromHandle(p->fileName,fileName,MAX_PATH_LEN);
	if( returnValueInt != 0 ){
		return returnValueInt;
	}

	if(pSession == NULL){
		outputToHistory("Could not create session object.");
		p->result = UNKOWN_ERROR;
		return 0;
	}
	
	ASSERT(pSession);

	wstringFilePath = CharPtrToWString(filePath);
	wstringFileName = CharPtrToWString(fileName);

	char buf[1000];
	sprintf(buf,"filename %s",WStringToString(wstringFileName).c_str());
	debugOutputToHistory(DEBUG_LEVEL,buf);

	sprintf(buf,"filepath %s",WStringToString(wstringFilePath).c_str());
	debugOutputToHistory(DEBUG_LEVEL,buf);

	bool retValue = pSession->loadResultSet(wstringFilePath,wstringFileName,true);

	if(!retValue){
		sprintf(buf,"File %s/%s is not readable.",WStringToString(wstringFilePath).c_str(),WStringToString(wstringFileName).c_str());
		outputToHistory(buf);
		p->result = FILE_NOT_READABLE;
		return 0;
	}

	pMyData->setResultFile(wstringFilePath,wstringFileName);
	p->result = SUCCESS;
	return 0;
}






static long RegisterFunction()
{
	/*	NOTE:
		Some XOPs should return a result of NIL in response to the FUNCADDRS message.
		See XOP manual "Restrictions on Direct XFUNCs" section.
	*/

	int funcIndex = GetXOPItem(0);		/* which function invoked ? */
	long returnValue = NIL;

	switch (funcIndex) {
		case 0:						
			returnValue = (long) checkForNewBricklets;
			break;
		case 1:						
			returnValue = (long) closeResultFile;
			break;
		case 2:						
			returnValue = (long) getAllBrickletData;
			break;
		case 3:						
			returnValue = (long) getAllBrickletMetaData;
			break;
		case 4:						
			returnValue = (long) getBrickletData;
			break;
		case 5:						
			returnValue = (long) getBrickletDimension;
			break;
		case 6:						
			returnValue = (long) getBrickletMetaData;
			break;
		case 7:						
			returnValue = (long) getBrickletRawData;
			break;
		case 8:						
			returnValue = (long) getBrickletViewTypeCode;
			break;
		case 9:						
			returnValue = (long) getErrorMessage;
			break;
		case 10:						
			returnValue = (long) getNumberOfBricklets;
			break;
		case 11:						
			returnValue = (long) getRangeBrickletData;
			break;
		case 12:						
			returnValue = (long) getRangeBrickletMetaData;
			break;
		case 13:						
			returnValue = (long) getResultFileMetaData;
			break;
		case 14:						
			returnValue = (long) getResultFileName;
			break;
		case 15:						
			returnValue = (long) getResultFilePath;
			break;
		case 16:						
			returnValue = (long) getVernissageVersion;
			break;
		case 17:						
			returnValue = (long) getXOPVersion;
			break;
		case 18:						
			returnValue = (long) openResultFile;
			break;
	}

	return returnValue;
}


/*	XOPEntry()

	This is the entry point from the host application to the XOP for all messages after the
	INIT message.
*/

// FIXME add cleanup message support

static void XOPEntry(void)
{	
	long result = 0;

	switch (GetXOPMessage()) {
		case FUNCADDRS:
			result = RegisterFunction();
			break;
	}
	SetXOPResult(result);
}

/*	main(ioRecHandle)

	This is the initial entry point at which the host application calls XOP.
	The message sent by the host must be INIT.
	main() does any necessary initialization and then sets the XOPEntry field of the
	ioRecHandle to the address to be called for future messages.
*/


HOST_IMPORT void
main(IORecHandle ioRecHandle)
{	
	XOPInit(ioRecHandle);							/* do standard XOP initialization */
	SetXOPEntry(XOPEntry);							/* set entry point for future calls */

	if (igorVersion < 600){
		SetXOPResult(REQUIRES_IGOR_600);
		return;
	}

	pMyData = new myData();
	if(pMyData == NULL){ // out of memory
		SetXOPResult(OUT_OF_MEMORY);
		return;
	}

	SetXOPResult(0L);

	return;
}
