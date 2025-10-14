/*
 * DataFile.h
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#ifndef DATAFILE_H_
#define DATAFILE_H_

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "statistics.h"

using namespace std;

class DataFile
{
private:
	ifstream m_filein;
	ofstream m_fileout;

	string m_nmfile_src;
	string m_nmfile_dst;

	vector<string> m_lines;
	Statistics m_stat;
	bool m_delline;

public:
	DataFile(const string& source_file, const string& dest_file, bool delline =
			false);
	virtual ~DataFile();

	bool GetData(void);
	bool PrepData(void);
	void PutData(void);
};

#endif /* DATAFILE_H_ */
