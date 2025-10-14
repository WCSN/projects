/*
 *  Thousand.h
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#ifndef THOUSAND_H_
#define THOUSAND_H_

#include <iostream>
#include <string>
#include <sys/types.h>

#include "DataFile.h"
#include "Engine.h"
#include "Statistics.h"

using namespace std;

class ThousandApp
{
private:

	DataFile *m_rules;
	Engine *m_eng;
	StatisticsCon *m_statcon; //Store info about Con
	StatisticsAll *m_statall; //Store info about all games

public:
	ThousandApp(void);
	virtual ~ThousandApp();

	void Run(void);
};

#endif /* THOUSAND_H_ */
