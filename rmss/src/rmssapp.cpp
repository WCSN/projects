/*
 * rmssapp.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#include "rmssapp.h"

Rmss::Rmss(const std::string& source_file, const std::string& dest_file, bool delline)
{
	fld = new DataFile(source_file, dest_file, delline);
}

Rmss::~Rmss()
{
    cout << " Done" << endl;
	delete fld;
}

int Rmss::Run(void)
{
	if (fld->GetData())
	{
		if (fld->PrepData())
			fld->PutData();
		else
			cout << "Not changed." << endl;
	}
	else
	{
		cout << "Error." << endl;
        return -1;
    }
    
return 0;    
}
