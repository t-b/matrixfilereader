/*
	The file dllhandler.h is part of the "MatrixFileReader XOP".
	It is licensed under the LGPLv3 with additional permissions,
	see License.txt	in the source folder for details.
*/
#pragma once

#include "ForwardDecl.h"

/*
	Everything related to loading the Vernissage DLLs.
	Calling closeSession() before exiting the application is mandatory.
*/
class DLLHandler{

	typedef Vernissage::Session * (*GetSessionFunc) ();
	typedef void (*ReleaseSessionFunc) ();

	public:
		// de/-constructors
		DLLHandler();
		~DLLHandler();

		// functions
		Vernissage::Session* createSessionObject();
		void closeSession();

		// const functions
		const std::string& getVernissageVersion()const{ return m_vernissageVersion; };

	private:
		void setLibraryPath();

	private:
		// variables
		GetSessionFunc m_pGetSessionFunc;
		ReleaseSessionFunc  m_pReleaseSessionFunc;
		HMODULE m_foundationModule;
		std::string m_vernissageVersion;
};