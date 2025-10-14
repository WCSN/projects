/*
 *  Statistics.cpp
 *
 *  Created on: Nov 22, 2011
 *      Author: wocson
 */

#include <fstream>
#include "Statistics.h"

using namespace std;

StatisticsCon::StatisticsCon(void)
{

}

void StatisticsCon::SaveData(void)
{
	m_stcon.as=0;
	m_stcon.qw=1;
	m_stcon.name="TTTT";
	string save_ststfile = "./.config/thousand/savestat.dat";
	fstream savestat;
	savestat.exceptions( fstream::failbit | fstream::badbit | fstream::goodbit );
	try
	{
		savestat.open(save_ststfile.c_str(), fstream::out | fstream::binary);
		savestat.write(reinterpret_cast<char*>(this), sizeof(this));
		savestat.close();

		StatData test;

		savestat.open(save_ststfile.c_str(), fstream::in | fstream::binary);
		savestat.read(reinterpret_cast<char*>(&test), sizeof(test));
		savestat.close();

	} catch (fstream::failure &e)
	{
		cerr << "Exception.\n" << e.what() << "\n" << endl;
	}
}

StatisticsCon::~StatisticsCon()
{
	SaveData();
	cout <<"Statisics:" << endl;
	cout <<"Saved..." << endl;
}

StatisticsAll::StatisticsAll(void)
{

}

StatisticsAll::~StatisticsAll()
{
	cout <<"All: Not have..." << endl;
}
