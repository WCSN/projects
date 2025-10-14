/*
 *  Gamer.h
 *
 *  Created on: 06.06.2013
 *      Author: wocson
 */

#ifndef GAMER_H_
#define GAMER_H_

#include <sys/types.h>
#include <string>
#include <iostream>
#include <vector>

#include "Card.h"

using namespace std;

class Gamer
{
private:

public:
	string m_name; //name Gamers
	bool m_human;
	u_short m_points;
	u_short m_pointCon;
	u_short m_distribute;
	u_short m_respawn;
	u_short m_bolts;
	u_short m_cast;

	u_short m_currentAucPoints;

	bool m_CupHiddenCards;
	bool m_ShowCards;

	bool m_MrHerts;
	bool m_MrDiamonds;
	bool m_MrClubs;
	bool m_MrSpades;
	bool m_MrAce;
	bool m_AucMember;

	ushort m_SumWeight;
	ushort m_SumPoints;
	ushort m_SumPoints_hearts;		//♥ червы — hearts
	ushort m_SumPoints_diamonds;	//♦ бубны — diamonds
	ushort m_SumPoints_clubs;		//♣ трефы — clubs
	ushort m_SumPoints_spades;		//♠ пики  — spades
	ushort m_RiskRating;
	ushort m_CommonRating;
	ushort m_ForthNine;
	ushort m_maxPoints;				// max gamer points

	vector<Card> m_CardsHands, m_CardHearts, m_CardDiamonds, m_CardClubs,
			m_CardSpades;

	vector<pair<ushort, ushort> > m_minline;
	vector<pair<ushort, ushort> > m_maxline;

	//Gamer(void);
	Gamer(const string& name, bool human = true);
	//Gamer(const string name, bool human, ushort points, ushort distribute,
	//		ushort respawn, ushort bolts, ushort cast);

	ushort m_Pos_in_Game;
	ushort m_Pos_in_Con;

	void SetPosinGame(ushort pos);
	void SetPosinCon(ushort pos);
	ushort GetPosinCon(void)
	{
		return m_Pos_in_Con;
	}
	;

	void CardStatusCheck(void);
	void AssessmentCards(void);
	void CleanHandCards(void);
	bool Analisys(const string& str);
	void Clear(void);

	void ShowStatus(void);

	virtual ~Gamer();
};

#endif /* GAMER_H_ */
