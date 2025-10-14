/*
 * Rmss.h
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#ifndef RMSS_H_
#define RMSS_H_

#include <iostream>
#include <string>
#include "datafile.h"

using namespace std;

class Rmss
{
private:
	DataFile *fld;
public:
	Rmss(const string& source_file, const string& dest_file, bool delline =
			false);
	virtual ~Rmss();

	int Run(void);
};

void help(void);
const char *version(void);

#endif /* RMSS_H_ */
