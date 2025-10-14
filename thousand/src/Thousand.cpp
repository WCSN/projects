/*
 * Thousand.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#include <unistd.h>
#include <stdlib.h>
#include "Thousand.h"
#include "Exception.h"

using namespace std;

ThousandApp::ThousandApp(void)
{
	string cnffile = "../.config/thousand/thousand.conf";
	string svffile = "../.config/thousand/save.dat";

	m_rules = new DataFile;
	try
	{
		if(!m_rules->ThcnfFiles(cnffile,svffile))
			throw mt::FileAccessDeniedException(cnffile);
	}
	catch(mt::Exception &e)
	{
		cout << "Error: config files not find.\n" << e.className() <<"\n" << e.displayText()<< endl;
		exit(1);
	}

	srand(unsigned(time(0)));

	m_eng = new Engine(m_rules);
	m_statcon = new StatisticsCon; //Store info about Con
	m_statall = new StatisticsAll; //Store info about all games
}

ThousandApp::~ThousandApp()
{
	delete m_statall;
	delete m_statcon;
	delete m_eng;
	delete m_rules;
}

void ThousandApp::Run(void)
{
	while(m_eng->Start())
	{
		sleep(1);
		srand(unsigned(time(0)));
	}
}
