#pragma once

#include "header.h"

typedef Vernissage::Session * (*GetSessionFunc) ();
typedef void (*ReleaseSessionFunc) ();

class DllStuff{

	public:
		// de/-constructors
		DllStuff();
		~DllStuff();

		// functions
		Vernissage::Session* createSessionObject();
		void closeSession();
		std::string getVernissageVersion(){ return m_vernissageVersion;};

	private:
		// variables
		GetSessionFunc m_pGetSessionFunc;
		ReleaseSessionFunc  m_pReleaseSessionFunc;
		HMODULE m_foundationModule;
		std::string m_vernissageVersion;

};
