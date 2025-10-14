/*
 * n_df.cpp
 *
 *  Created on: 06.12.2012
 *      Author: wocson
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/stat.h>

#include<sys/param.h>
#include<sys/ucred.h>
#include<sys/mount.h>
#include<errno.h>
#include<string.h>
#include<errno.h>
#include <unistd.h>

#include "n_df.h"

using namespace std;


std::string itoa(int value, int base)
{

	std::string buf;

	// check that the base if valid
	if (base < 2 || base > 16)
		return buf;

	enum
	{
		kMaxDigits = 35
	};

	buf.reserve(kMaxDigits); // Pre-allocate enough space.

	int quotient = value;

	// Translating number to string with base:
	do
	{
		buf += "0123456789abcdef"[std::abs(quotient % base)];
		quotient /= base;
	} while (quotient);

	// Append the negative sign
	if (value < 0)
		buf += '-';

	std::reverse(buf.begin(), buf.end());
	return buf;
}

struct dfdata
{
	uint64_t correctbz;
	uint64_t blksize;
	uint64_t disk_size;
	uint64_t disk_used;
	uint64_t disk_free;
	int capacity;
	string fnPath;
};

vector<dfdata> dfv;

void outdata(void)
{
	cout << "BlockSize XXX         Size         Used        Avail Capacity Mounted on" << endl;
	cout.flush();

	vector<dfdata>::iterator itl;

	for (itl = dfv.begin(); itl != dfv.end(); itl++)
	{
		//cout << " " << setw(itoa(disk_size,10).length()+10) << right << disk_size;
		cout << setw(9) << itl->blksize << " " << setw(3) << itl->correctbz;
		cout << " " << setw(12) << itl-> disk_size << " " << setw(12) << itl->disk_used;
		cout << " " << setw(12) << itl->disk_free << " "  << setw(7) << itl->capacity << "% ";
		cout << itl->fnPath << endl;
	}
}

void n_df(int argc, char** argv)
{
	struct statvfs fiData;
//	struct statvfs *fpData;

	dfdata df;

	for (int i = 1; i < argc; i++)
	{
		df.fnPath.assign(argv[i]);

		if ((statvfs(df.fnPath.c_str(), &fiData)) < 0)
		{
			cout << "Failed to stat" << df.fnPath << endl;
		}
		else
		{
			struct stat fi;
			stat(df.fnPath.c_str(), &fi);

			int corr = fi.st_blksize / 512;

			if(corr == 32 || corr == 8)	corr = 1;
			else if(corr == 64 ) corr = 8;

			df.correctbz = fi.st_blksize / 512;

			df.blksize 	= fi.st_blksize;
			df.disk_size = fiData.f_blocks * corr;	// blocks def 512 байт
			df.disk_free = fiData.f_bavail * corr;

			df.disk_used = df.disk_size - df.disk_free;
			df.capacity 	 = (int)((df.disk_used*100)/df.disk_size);
		}

		dfv.push_back(df);
		df.fnPath.clear();
	}

	outdata();
}


