/*
 * DataFile.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "Thousand.h"

using namespace std;

DataFile::DataFile(void)
{

}

bool DataFile::ThcnfFiles(const string& confname_file, const std::string& save_file)
{
	m_conf_file = confname_file;
	m_save_file = save_file;
	string line;

	m_filestream.open(m_conf_file.c_str(), ios::in);

	if(!m_filestream.fail())
	{
		cout << " Read config file ... ";

		while (getline(m_filestream, line))
		{
			if(line[0]=='#' || line[0]=='\0') continue;
			m_confdata.push_back(line);
			//cout << line << endl;
		}

		m_filestream.close();

		ParseRulesData();

		return true;
	}
	else
		return false;
}

string DataFile::getValParameter(string namepar)
{
	namepar += "=";
	string retpar = "none";

	for(auto icl : m_confdata)
	{
		if(icl.find(namepar) == 0)
		{
			retpar.clear();
			icl = icl.substr(namepar.length(),icl.length());

			for(uint i = 0; i < icl.length(); i++)
			{
				if(!isspace(icl[i]) && icl[i]!='#')
					retpar+=icl[i];
				else
					break;
			}
		return retpar;
		}
	}

	if(retpar=="none")
	{
		cout << "Parameter: "<< namepar << " in " << m_conf_file << " not find...\n Program close." << endl;
		exit(1);
	}

	return retpar; //
}

void DataFile::ParseRulesData(void)
{
	// Rules Правила
	// Limits Ограничения
	m_opt.DropfromCask = ((getValParameter("DropfromCask")=="yes")?true:false);
	//false; 		// drop from cask
	m_opt.JumpOver5 = ((getValParameter("JumpOver5")=="yes")?true:false);
	// jump over 5 for Auction
	m_opt.CaskCount = atoi(getValParameter("CaskCount").c_str());
	// 880;

	// Penalties Штрафы
	m_opt.DroptoZero = ((getValParameter("DroptoZero")=="yes")?true:false);
	// -555 | 555
	m_opt.DownifZeroThreeBolt = ((getValParameter("DownifZeroThreeBolt")=="yes")?true:false);
	// -120 if 3 zero con
	m_opt.ZeroThreefailCask = ((getValParameter("ZeroThreefailCask")=="yes")?true:false);
	// 3 cask drop to 0

	// Respawn Пересдача
	m_opt.RespawnHidden = atoi(getValParameter("RespawnHidden").c_str());
	// 4
	m_opt.RespawnHands = atoi(getValParameter("RespawnHands").c_str());
	// 14
	m_opt.Respawn49 = ((getValParameter("Respawn49")=="yes")?true:false);
	// 4 nine in hands
	m_opt.Respawn29Hidden = ((getValParameter("Respawn29Hidden")=="yes")?true:false);
	// 2 nine in hidden

	// Distribute Роспись
	m_opt.DistPoints = atoi(getValParameter("DistPoints").c_str());
	// 60 point to gamer
	m_opt.DistNo = ((getValParameter("DistNo")=="yes")?true:false);
	// no penalties  if Dist
	m_opt.Dist3sbys = ((getValParameter("Dist3sbys")=="yes")?true:false);
	// -120 if 3 Dist step-by-step
	m_opt.DistifCask = ((getValParameter("DistifCask")=="yes")?true:false);
	// if Cask can dist

	// Trumps Козыри
	m_opt.AceMariage = ((getValParameter("AceMariage")=="yes")?true:false);
	// Ace Marriage
	m_opt.AddedCostHidden4 = ((getValParameter("AddedCostHidden4")=="yes")?true:false);
	// Added Cost if 4 gamers

	m_opt.nameGamers.push_back(getValParameter("Gamer1"));
	m_opt.nameGamers.push_back(getValParameter("Gamer2"));
	m_opt.nameGamers.push_back(getValParameter("Gamer3"));

	if(atoi(getValParameter("NGamers").c_str())==4)
		m_opt.nameGamers.push_back(getValParameter("Gamer4"));

	m_opt.Lang = getValParameter("Language");
}

void DataFile::SaveData(void)
{
	m_savestfile.exceptions( fstream::failbit | fstream::badbit | fstream::goodbit );
	try
	{
		m_savestfile.open(m_save_file.c_str(), fstream::out | fstream::binary);
		m_savestfile.write(reinterpret_cast<char*>(&m_opt), sizeof(m_opt));
		m_savestfile.close();

		Options test;

		m_savestfile.open(m_save_file.c_str(), fstream::in | fstream::binary);
		m_savestfile.read(reinterpret_cast<char*>(&test), sizeof(test));
		m_savestfile.close();

	} catch (fstream::failure &e)
	{
		cerr << "Exception.\n" << e.what() << "\n" << endl;
	}
}

DataFile::~DataFile()
{
	SaveData();
	cout << "Stored in " << m_save_file << "\nDone." << endl;
	m_filestream.close();
	m_savestfile.close();
}
