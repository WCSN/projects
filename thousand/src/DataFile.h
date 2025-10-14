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
#include <vector>

using namespace std;

struct Options
{
public:
// Rules Правила
// Limits Ограничения
	bool DropfromCask; 		// drop from cask
	bool JumpOver5;			// jump over 5 for Auction
	uint CaskCount; 		// 880;
// Penalties Штрафы
	bool DroptoZero; 		// -555 | 555
	bool DownifZeroThreeBolt;		// -120 if 3 zero con
	bool ZeroThreefailCask; 	// 3 cask -880
// Respawn Пересдача
	ushort RespawnHidden; 	// 4
	ushort RespawnHands;  	// 14
	bool Respawn49;     	// 4 nine in hands
	bool Respawn29Hidden;  	// 2 nine in hidden
// Distribute Роспись
	ushort DistPoints; 		// 60 point to gamer
	bool DistNo;			// no minus  if Dist
	bool Dist3sbys;			// -120 if 3 Dist step-by-step
	bool DistifCask;		// if Cask can dist
// Trumps Козыри
	bool AceMariage;		// Ace Marriage
	bool AddedCostHidden4;	// Added Cost if 4 gamers
// Gamers Игроки
	vector <string> nameGamers;

// Options Настройки
	string Lang;
};

class DataFile
{
private:
	fstream m_filestream;
	fstream m_savestfile;

	string m_conf_file;
	string m_save_file;
	vector <string> m_confdata;

	string getValParameter(string namepar);

public:
	Options m_opt;

public:
	DataFile(void);
	virtual ~DataFile();

	bool ThcnfFiles(const string& confname_file, const std::string& save_file);
	void ParseRulesData(void);
	void SaveData(void);
};

#endif /* DATAFILE_H_ */
