/*
 * Statistics.h
 *
 *  Created on: Nov 22, 2011
 *      Author: wocson
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <iostream>

using namespace std;

class Statistics
{
public:
	bool m_floutinfo;
	unsigned long m_space;	// del spases
	unsigned long m_tab;	// del tabs

	unsigned long m_lns;	// all lines
	unsigned long m_lnsr;	// correct lines
	unsigned long m_dlns;	// removed lines

	Statistics(void);
	virtual ~Statistics();

private:
	void OutStat(void);
};

#endif /* STATISTICS_H_ */
