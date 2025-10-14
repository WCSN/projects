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

struct StatData
{
	uint as;
	ushort qw;
	string name;
};

class StatisticsCon
{
	StatData m_stcon;

public:

	StatisticsCon(void);
	virtual ~StatisticsCon();

private:

	void SaveData(void);
};

class StatisticsAll
{
public:
	StatisticsAll(void);
	virtual ~StatisticsAll();

};

#endif /* STATISTICS_H_ */
