/*
 * wlsystem.h
 *
 *  Created on: 07.06.2013
 *      Author: wocson
 */

#ifndef WLSYSTEM_H_
#define WLSYSTEM_H_

#include <vector>
#include <random>

using namespace std;

//template< class RandomIt >
//void random_shuffle(RandomIt first, RandomIt last);

int getch(void);

vector<uint> randomm(uint minVal, uint maxVal, uint count=0, uint step=1,bool srt=false);

bool YesNo(const string& str);
bool YesNo(const char**);

#endif /* WLSYSTEM_H_ */
