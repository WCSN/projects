/*
 * datafile.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#include "rmssapp.h"

DataFile::DataFile(const string &source_file, const string &dest_file, bool delline) :
m_nmfile_src(source_file), m_nmfile_dst(dest_file), m_delline(delline)
{
}

bool DataFile::GetData(void)
{
	string line;
	m_filein.open(m_nmfile_src.c_str());
	
	if (!m_filein)  
    {
        cout << "Do not open file: " << m_nmfile_src << endl; 
        return false;
    }

	cout << " - Get data from: " << m_nmfile_src  << endl;

	while (getline(m_filein, line))
	{
		for (auto isymb : line)
		{
			if (isymb == 0x00)
			{
				cout << "Binary file?" << endl;
				m_stat.m_floutinfo = false;
				return false;
			}
		}

		if (m_delline && line.size() == 0)
		{
			++m_stat.m_dlns;
			continue;
		}
		m_lines.push_back(line);
	}

return true;
}

bool DataFile::PrepData(void)
{
	cout << " - Analising data" << endl;
	m_stat.m_lns = m_lines.size();

	vector<string>::iterator itl;

	for (itl = m_lines.begin(); itl != m_lines.end(); ++itl)
	{
		string::iterator its;

		bool lnfcfind = false;

		for (its = itl->end()-1; its >= itl->begin(); --its)
		{
			if ((*its) == 0x20 || (*its) == 0x09)
			{
				((*its) == 0x20) ? ++m_stat.m_space : ++m_stat.m_tab;
				itl->erase(its);
				lnfcfind = true;
			}
			else
				break;
		}
		if (lnfcfind)
			++m_stat.m_lnsr;
	}

	if (m_stat.m_lnsr == 0 && m_stat.m_dlns == 0)
		return false;

return true;
}

void DataFile::PutData(void)
{
	m_fileout.open(m_nmfile_dst.c_str());

	if (!m_fileout)  
    {
        cout << "The file cannot be opened " << m_nmfile_dst << endl;
        return;
    }

	cout << " - Save data to: " << m_nmfile_dst << endl;

	for (auto itl : m_lines)
	    m_fileout << itl << endl;
}

DataFile::~DataFile()
{
	m_filein.close();
	m_fileout.close();
}
