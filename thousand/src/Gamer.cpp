/*
 *  Gamer.cpp
 *
 *  Created on: 06.06.2013
 *      Author: wocson
 */

#include <iostream>     // std::cout
#include <algorithm>    // std::min_element, std::max_element
#include "Gamer.h"

using namespace std;

Gamer::Gamer(const string& name, bool human) :
		m_name(name), m_human(human), m_points(0), m_pointCon(0), m_distribute(0),
		m_respawn(0), m_bolts(0), m_cast(0), m_currentAucPoints(0), m_CupHiddenCards(0),
		m_ShowCards(true), m_MrHerts(false), m_MrDiamonds(false), m_MrClubs(false),
		m_MrSpades(false), m_MrAce(false), m_AucMember(false), m_SumWeight(0), m_SumPoints(0),
		m_SumPoints_hearts(0), m_SumPoints_diamonds(0), m_SumPoints_clubs(0),
		m_SumPoints_spades(0), m_RiskRating(0), m_CommonRating(0), m_ForthNine(0),
		m_maxPoints(120), m_Pos_in_Game(0), m_Pos_in_Con(0)
{

}


Gamer::~Gamer()
{
	// TODO Auto-generated destructor stub
}

void Gamer:: Clear(void)
{
	m_SumWeight = m_RiskRating = m_SumPoints = m_CommonRating = m_ForthNine = 0;
	m_maxPoints = 120;

	m_CardHearts.clear();
	m_CardDiamonds.clear();
	m_CardClubs.clear();
	m_CardSpades.clear();

	m_MrHerts = m_MrDiamonds = m_MrClubs = m_MrSpades = m_MrAce = false;
}


void Gamer::CardStatusCheck(void)
{
	Clear();

	bool HK, DK, CK, SK, HQ, DQ, CQ, SQ, HA, DA, CA, SA, Ten, Ace;
	HK = DK = CK = SK = HQ = DQ = CQ = SQ = HA = DA = CA = SA = Ten = Ace = false;

	for (auto it : m_CardsHands)
	{
		m_SumPoints += it.m_points;

		if (it.m_points == 0)
			m_ForthNine++;

		if (it.m_points == 10)
		{
			Ten = true;
			m_RiskRating++;
			m_CommonRating++;
		}

		if (it.m_points == 11)
		{
			Ace = true;
			m_RiskRating++;
			m_CommonRating++;
		}

		// ♥♡C/1 червы—hearts, ♦♢B/2 бубны — diamonds, ♣♧K/3 трефы, крести—clubs, ♠♤V/4 пики,вини—spades
		switch (it.m_lear)
		{
		case 1:
			m_CardHearts.push_back(it);
			m_SumPoints_hearts += it.m_points; //♥ червы — hearts

			if (it.m_points == 4)
			{
				HK = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			if (it.m_points == 3)
			{
				HQ = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			if (it.m_points == 11)
			{
				HA = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			break;
		case 2:
			m_CardDiamonds.push_back(it);
			m_SumPoints_diamonds += it.m_points; //♦ бубны — diamonds

			if (it.m_points == 4)
			{
				DK = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			if (it.m_points == 3)
			{
				DQ = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			if (it.m_points == 11)
			{
				DA = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			break;
		case 3:
			m_CardClubs.push_back(it);
			m_SumPoints_clubs += it.m_points; //♣ трефы — clubs

			if (it.m_points == 4)
			{
				CK = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			if (it.m_points == 3)
			{
				CQ = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			if (it.m_points == 11)
			{
				CA = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			break;
		case 4:
			m_CardSpades.push_back(it);
			m_SumPoints_spades += it.m_points; //♠ пики — spades

			if (it.m_points == 4)
			{
				SK = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			if (it.m_points == 3)
			{
				SQ = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			if (it.m_points == 11)
			{
				SA = true;
				m_RiskRating++;
				m_CommonRating++;
			}
			break;
		default:
			// never CASE!!!
			// kill same quickly
			break;
		}
	}

	if (HK && HQ)
	{
		m_MrHerts = true;
		m_SumPoints += 100;
		m_RiskRating--;
		m_CommonRating++;
		m_maxPoints+=100;
	}

	if (DK && DQ)
	{
		m_MrDiamonds = true;
		m_SumPoints += 80;
		m_RiskRating--;
		m_CommonRating++;
		m_maxPoints+=80;
	}

	if (CK && CQ)
	{
		m_MrClubs = true;
		m_SumPoints += 60;
		m_RiskRating--;
		m_CommonRating++;
		m_maxPoints+=60;
	}

	if (SK && SQ)
	{
		m_MrSpades = true;
		m_SumPoints += 40;
		m_RiskRating--;
		m_CommonRating++;
		m_maxPoints+=40;
	}

	if (HA && DA && CA && SA && Ten)
	{
		m_MrAce = true;
		m_SumPoints += 200;
		m_RiskRating--;
		m_CommonRating++;
		m_maxPoints+=200;
	}

	m_SumWeight = m_SumPoints + m_CommonRating;
	m_AucMember=true;

	AssessmentCards();
}

bool sortfuncCard (const Card i, const Card j) { return (i.m_points < j.m_points); }
bool funcHC(const Card i, const Card j) { return i.m_points < j.m_points; }

void Gamer::AssessmentCards(void)
{
	sort(m_CardSpades.begin(),m_CardSpades.end(),sortfuncCard);
	sort(m_CardClubs.begin(),m_CardClubs.end(),sortfuncCard);
	sort(m_CardDiamonds.begin(),m_CardDiamonds.end(),sortfuncCard);
	sort(m_CardHearts.begin(),m_CardHearts.end(),sortfuncCard);

	m_CardsHands.clear();

	for (auto it : m_CardSpades)	m_CardsHands.push_back(it);
	for (auto it : m_CardClubs)		m_CardsHands.push_back(it);
	for (auto it : m_CardDiamonds)	m_CardsHands.push_back(it);
	for (auto it : m_CardHearts)	m_CardsHands.push_back(it);

	m_minline.clear();
	m_maxline.clear();

	ushort sz = m_CardSpades.size();
	if(sz>0)
	{
		m_minline.push_back(make_pair(sz,min_element(m_CardSpades.begin(),m_CardSpades.end(),funcHC)->m_numcod));
		m_maxline.push_back(make_pair(sz,max_element(m_CardSpades.begin(),m_CardSpades.end(),funcHC)->m_numcod));
	}

	sz = m_CardClubs.size();
	if(sz>0)
	{
		m_minline.push_back(make_pair(sz,min_element(m_CardClubs.begin(),m_CardClubs.end(),funcHC)->m_numcod));
		m_maxline.push_back(make_pair(sz,max_element(m_CardClubs.begin(),m_CardClubs.end(),funcHC)->m_numcod));
	}

	sz = m_CardDiamonds.size();
	if(sz>0)
	{
		m_minline.push_back(make_pair(sz,min_element(m_CardDiamonds.begin(),m_CardDiamonds.end(),funcHC)->m_numcod));
		m_maxline.push_back(make_pair(sz,max_element(m_CardDiamonds.begin(),m_CardDiamonds.end(),funcHC)->m_numcod));
	}

	sz = m_CardHearts.size();
	if(sz>0)
	{
		m_minline.push_back(make_pair(sz,min_element(m_CardHearts.begin(),m_CardHearts.end(),funcHC)->m_numcod));
		m_maxline.push_back(make_pair(sz,max_element(m_CardHearts.begin(),m_CardHearts.end(),funcHC)->m_numcod));
	}
}

const string LANG = "ru_RU";
//const string LANG = "en_US";

void Gamer::ShowStatus(void)
{
	ushort numCards = m_CardsHands.size();

	//cout << "\n";
	if (LANG == "ru_RU")
		cout << m_name << (m_CupHiddenCards?" (Взял прикуп)":" ") <<"\n" << m_Pos_in_Game << " " << m_Pos_in_Con
		<<"|Очки:" << m_points << "|Росписи:"<< m_distribute << "|Пересдачи:"
		<< m_respawn << "|Болты:"<< m_bolts << "|Бочки:" << m_cast << "|";

	cout << "\n";

	ushort i = 0;
	for (i = 0; i < numCards; ++i) cout << "┌───┐";

	if (m_ShowCards)
	{
		cout << " MP:"<<m_maxPoints<<" SW:" << m_SumWeight << " SP:" << m_SumPoints
			<< " CR:" << m_CommonRating << " RR:" << m_RiskRating << "\n";

		for (auto it : m_CardsHands) it.ShowCard();

		cout << " M: " << (m_MrHerts?"HM ":"") << (m_MrDiamonds?"DM ":"")
				       << (m_MrClubs?"CM ":"") << (m_MrSpades?"SM ":"")
				       << (m_MrAce?"AM ":" ");

		cout << "Mn: ";
		for (auto im : m_minline)
		{
			if(im.first > 0)
				cout << im.first << "-" << im.second << " ";
			else if(im.first <= 0)
				continue;
		}

		cout << "Mx: ";
		for (auto im : m_maxline)
		{
			if(im.first > 0)
				cout << im.first << "-" << im.second << " ";
			else if(im.first <= 0)
				continue;
		}
	}

	if(!m_ShowCards)
	{
		cout << "\n";
		for (i = 0; i < numCards; ++i) cout << "|***|";
	}
	cout << "\n";

	for (i = 0; i < numCards; ++i) cout << "└───┘";
	cout << endl;
}


void Gamer::CleanHandCards(void)
{
	m_CardsHands.clear();
	m_CupHiddenCards = false;
}

void Gamer::SetPosinGame(ushort pos)
{
	m_Pos_in_Game = pos;
}

void Gamer::SetPosinCon(ushort pos)
{
	m_Pos_in_Con = pos;
}

bool Gamer::Analisys(const string& str)
{
	if(1)//!!! Добавить анализ!
	{
		cout << str << "Пересдаю...";
		return 1;
	}
return 0;
}
