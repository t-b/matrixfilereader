#pragma once

#include <map>

#include "dllhandler.h"
#include "constants.h"
#include "brickletClass.h"

typedef	std::map<int, BrickletClass*, std::less<int>>		IntBrickletClassPtrMap;

class GlobalData{

public:
	// constructors/deconstructors
	GlobalData();
	~GlobalData();
	
public:
	// functions

	// return the filename and the dirPath of the currently loaded result set
	std::string getFileName();
	std::string getDirPath();
	void setResultFile(char *dirPath, char *fileName){ this->setResultFile(std::string(dirPath),std::string(fileName));}
	void setResultFile( std::string dirPath, std::string fileName);
	bool resultFileOpen();
	Vernissage::Session* getVernissageSession();
	std::string getVernissageVersion();
	void closeSession();
	void closeResultFile();
	BrickletClass* getBrickletClassObject(int brickletID);
	void createBrickletClassObject(int brickletID, void *pBricklet);
	void setError(int errorCode, std::string msgArgument = std::string());
	void setInternalError(int errorCode);
	int getLastError(){ return m_lastError; }
	std::string getLastErrorMessage();

	void readSettings();
	// debug
	bool debuggingEnabled(){ return m_debug; };
	
	// double
	bool doubleWaveEnabled(){ return m_doubleWave; };
	int getIgorWaveType();

	// datafolder
	bool datafolderEnabled(){ return m_datafolder; };

	// overwrite
	bool overwriteEnabled(){ return m_overwrite; };
	int overwriteEnabledAsInt(){ return int(m_overwrite); };

	// variables
	char outputBuffer[ARRAY_SIZE];

private:
	bool m_debug,m_doubleWave, m_datafolder, m_overwrite;
	std::string m_resultFileName, m_resultFilePath;
	Vernissage::Session *m_VernissageSession;
	DLLHandler *m_DLLHandler;
	int m_lastError;
	std::string m_lastErrorArgument;
	IntBrickletClassPtrMap		m_brickletIDBrickletClassMap;

	void setLastError(int errorCode, std::string argument = std::string());
	std::string getLastErrorArgument(){ return m_lastErrorArgument;}

	void enableDatafolder(bool var){ m_datafolder = var; };
	void enableDoubleWave(bool var){ m_doubleWave = var; };
	void enableDebugging(bool var){ m_debug=var; };
	void enableOverwrite(bool var){ m_overwrite = var; };

};

// declare global object globDataPtr
extern GlobalData *globDataPtr;