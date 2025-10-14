/*
 * Statistics.cpp
 *
 *  Created on: Nov 22, 2011
 *      Author: wocson
 */

#include "statistics.h"

Statistics::Statistics(void) :
m_floutinfo(true), m_space(0), m_tab(0), m_lns(0), m_lnsr(0), m_dlns(0)
{
}

void Statistics::OutStat(void)
{
	if (m_floutinfo)
	{
		cout << "─────────────────────────────────────────────\n"
		<< "Lines: " << m_lns << " correct: " << m_lnsr;
		if (m_dlns > 0)
		    cout << " removed: " << m_dlns;
		cout << "\nRemoved spaces: " << m_space << " tabs: " << m_tab
		<< endl;
    }
	else
		cout << "Break." << endl;
}

Statistics::~Statistics()
{
	OutStat();
}
