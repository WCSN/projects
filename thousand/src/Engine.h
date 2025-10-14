/*
 *  Engine.h
 *
 *  Created on: 04.06.2013
 *      Author: wocson
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <iostream>
#include <sys/types.h>
#include <string>

#include "DataFile.h"
#include "Card.h"
#include "Gamer.h"

using namespace std;

class Engine
{
//7private:

private: // Mariage ♥C/1 ♦B/2  ♣K/3  ♠V/4

	const DataFile *m_rules;
	unsigned long m_StepCon;
	ushort m_Thousand;
	ushort m_Trumps;			//козырь
	ushort m_countGamers;
	ushort m_shiftPos;

	ushort m_NineHid;
	ushort m_SumPointsHid;

	vector<Card> m_Cards0;
	vector<Card> m_CardsGame;
	vector<Card> m_HiddenCards;
	vector<Gamer> m_Gamers;


	void MixCard(void);
	void SpawnCard(void);
	void Auction(void);
	void Game(void);
	void Clean(void);
	 int Stop(void);

	 void CheckCardsGamers(void);
	 bool GamersStatus(void);
	 void ShowHidden(void);
	 void SpawnAfAuc(void);
	 void CorrectRate(void);

public:
Engine(const DataFile *rules);
	virtual ~Engine();

	bool Start(void);
private:

};

#endif /* ENGINE_H_ */
