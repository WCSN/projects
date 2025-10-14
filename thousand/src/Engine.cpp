/*
 *  Engine.cpp
 *
 *  Created on: 04.06.2013
 *      Author: wocson
 */

#include <random>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include "Engine.h"
#include "wlsystem.h"

using namespace std;

bool sortfuncGame(const Gamer i, const Gamer j)
{
	return (i.m_Pos_in_Game < j.m_Pos_in_Game);
}
bool sortfuncCon(const Gamer i, const Gamer j)
{
	return (i.m_Pos_in_Con < j.m_Pos_in_Con);
}

Engine::Engine(const DataFile *rules) :
		m_rules(rules), m_StepCon(0), m_Trumps(0), m_countGamers(0), m_shiftPos(
				0), m_NineHid(0), m_SumPointsHid(0)
{
	m_Thousand = 0;
	// ♥C/1 ♦B/2  ♣K/3  ♠V/4  ♤ ♧ ♡ ♢
	m_Cards0.push_back(Card(" 9", "♥", 1, 0, 100));
	m_Cards0.push_back(Card(" J", "♥", 1, 2, 102));
	m_Cards0.push_back(Card(" Q", "♥", 1, 3, 103));
	m_Cards0.push_back(Card(" K", "♥", 1, 4, 104));
	m_Cards0.push_back(Card("10", "♥", 1, 10, 110));
	m_Cards0.push_back(Card(" A", "♥", 1, 11, 111));

	m_Cards0.push_back(Card(" 9", "♦", 2, 0, 200));
	m_Cards0.push_back(Card(" J", "♦", 2, 2, 202));
	m_Cards0.push_back(Card(" Q", "♦", 2, 3, 203));
	m_Cards0.push_back(Card(" K", "♦", 2, 4, 204));
	m_Cards0.push_back(Card("10", "♦", 2, 10, 210));
	m_Cards0.push_back(Card(" A", "♦", 2, 11, 211));

	m_Cards0.push_back(Card(" 9", "♧", 3, 0, 300));
	m_Cards0.push_back(Card(" J", "♧", 3, 2, 302));
	m_Cards0.push_back(Card(" Q", "♧", 3, 3, 303));
	m_Cards0.push_back(Card(" K", "♧", 3, 4, 304));
	m_Cards0.push_back(Card("10", "♧", 3, 10, 310));
	m_Cards0.push_back(Card(" A", "♧", 3, 11, 311));

	m_Cards0.push_back(Card(" 9", "♠", 4, 0, 400));
	m_Cards0.push_back(Card(" J", "♠", 4, 2, 402));
	m_Cards0.push_back(Card(" Q", "♠", 4, 3, 403));
	m_Cards0.push_back(Card(" K", "♠", 4, 4, 404));
	m_Cards0.push_back(Card("10", "♠", 4, 10, 410));
	m_Cards0.push_back(Card(" A", "♠", 4, 11, 411));

	// ♥C/1 ♦B/2  ♣K/3  ♠V/4  ♤ ♧ ♡ ♢

	m_countGamers = m_rules->m_opt.nameGamers.size();
	vector<uint> r = randomm(1, m_countGamers);
	vector<string>::const_iterator it = m_rules->m_opt.nameGamers.begin();

	for (ushort i = 0; i < m_countGamers; ++i, ++it)
	{
		m_Gamers.push_back(Gamer(*it));
		(m_Gamers.end() - 1)->SetPosinGame(r[i]);
		(m_Gamers.end() - 1)->SetPosinCon(r[i]);
	}
}

Engine::~Engine()
{
	Clean();
}

void Engine::Clean(void)
{
	m_Gamers.clear();
	m_HiddenCards.clear();
	m_CardsGame.clear();
	m_Cards0.clear();
}

int Engine::Stop(void)
{
	cout << "END" << endl;
	return 0;
}

void Engine::MixCard(void)
{
	srand(unsigned(time(0)));
	random_shuffle(m_CardsGame.begin(), m_CardsGame.end());
}

void Engine::SpawnCard(void)
{
	cout << "───SpawnCards───────────────────────────────────────────" << endl;

	sort(m_Gamers.begin(), m_Gamers.end(), sortfuncCon);

	m_CardsGame = m_Cards0;
	MixCard();

	m_HiddenCards.clear();

	vector<Card>::const_iterator it;
	ushort i = 0;

	m_NineHid = m_SumPointsHid = 0;

	// Сброс карт в прикуп
	for (it = m_CardsGame.begin(); i < 3; ++it, ++i)
	{
		m_HiddenCards.push_back(*it);
		if (it->m_points == 0)
			m_NineHid++;
		m_SumPointsHid += it->m_points;
	}

	vector<Gamer>::iterator gi;

	for (gi = m_Gamers.begin(); gi < m_Gamers.end(); ++gi)
	{
		gi->CleanHandCards();
		ushort icard = 0;

		for (; it < m_CardsGame.end(); ++it)
		{
			if (icard >= 7)
			{
				icard = 0;
				break;
			}
			gi->m_CardsHands.push_back(*it);
			icard++;
		}
		gi->CardStatusCheck();
	}
}

void Engine::Auction(void)
{
	cout << "───Auction──────────────────────────────────────────────" << endl;

	ushort CurHidAuction = 100;
	ushort step = 5, i = 0;
	ushort aucmembers = m_countGamers;
	m_shiftPos = 0;
	vector<Gamer>::iterator gi, si;

	do
	{
		cout << i++ << ". ";

		for (gi = m_Gamers.begin(); gi < m_Gamers.end(); ++gi)
		{
			if (gi->m_AucMember)
			{
				cout << gi->m_name << ":";

				if (gi->m_maxPoints >= CurHidAuction) //Add classify risk !!!
				{
					gi->m_currentAucPoints = CurHidAuction;
					cout << gi->m_currentAucPoints << " ";
					m_shiftPos = gi->m_Pos_in_Game - 1;
					CurHidAuction += step; //Next Hid Points
					si = gi;
				}
				else
				{
					cout << "PASS ";
					gi->m_AucMember = false;
					aucmembers--;
				}

				if (aucmembers == 1)
					break;
			}
			else
				continue;
		}
		cout << endl;
	} while (aucmembers > 1);

	ShowHidden();

	vector<Card>::const_iterator it;
	for (it = m_HiddenCards.begin(); it < m_HiddenCards.end(); ++it)
		si->m_CardsHands.push_back(*it);
	si->m_CupHiddenCards = true;
	si->CardStatusCheck();

	sort(m_Gamers.begin(), m_Gamers.end(), sortfuncGame);
	rotate(m_Gamers.begin(), m_Gamers.begin() + m_shiftPos, m_Gamers.end()); //for Con position after Auction
}

bool minfuncAA(pair<ushort, ushort> i, pair<ushort, ushort> j)
{
	return i.first < j.first;
}

void Engine::SpawnAfAuc(void)
{
	vector<Gamer>::iterator gi = m_Gamers.begin();
	vector<pair<ushort, ushort> >::iterator im = min_element(gi->m_minline.begin(), gi->m_minline.end(), minfuncAA);
	vector<Card>::iterator ci;

	for (ci = gi->m_CardsHands.begin(); ci < gi->m_CardsHands.end(); ++ci)
		if (ci->m_numcod == im->second)
		{
			(gi + 1)->m_CardsHands.push_back(*ci);
			gi->m_CardsHands.erase(ci);
			break;
		}

	gi->m_minline.erase(im);

	im = min_element(gi->m_minline.begin(), gi->m_minline.end(), minfuncAA);

	for (ci = gi->m_CardsHands.begin(); ci < gi->m_CardsHands.end(); ++ci)
		if (ci->m_numcod == im->second)
		{
			(gi + 2)->m_CardsHands.push_back(*ci);
			gi->m_CardsHands.erase(ci);
			break;
		}

	CheckCardsGamers();
}

void Engine::CorrectRate(void)
{
	cout << "───Correct──────────────────────────────────────────────" << endl;
	cout << "Correct....+++++" << endl;
}

void Engine::Game(void)
{
	cout << "───Game─────────────────────────────────────────────────" << endl;
//begin
	m_Trumps = 1;

//ending

	for (auto gi : m_Gamers)
	{
		gi.m_Pos_in_Con++;
		if (gi.m_Pos_in_Con > m_countGamers)
			gi.m_Pos_in_Con = 1;
	}

	cout << endl;
}

void Engine::CheckCardsGamers(void)
{
	cout << "───CheckCardsGamers──────────────────────────────────────" << endl;
	for (auto gi : m_Gamers)
		gi.CardStatusCheck();
}

bool Engine::GamersStatus(void)
{
	bool retval = false;
	cout << "───GamersStatus──────────────────────────────────────────" << endl;

	for (auto gi : m_Gamers)
	{
		gi.ShowStatus();

		// проверка на то что взят прикуп
		if (gi.m_CupHiddenCards)
		{
			//проверка суммы очков в прикупе
			if (gi.m_SumPoints <= m_rules->m_opt.RespawnHidden)
			{
				ostringstream buff;
				buff << " Сумма очков в прикупе " << gi.m_SumPoints << "!";

				if (gi.m_human)
				{
					buff << "\nПересдать?";
					retval = YesNo(buff.str());
				}
				else
					retval = gi.Analisys(buff.str());

				if (retval)
					break;
			}

			// проверки валидности карт в прикупе
			if (m_NineHid >= 2 && m_rules->m_opt.Respawn29Hidden)
			{
				ostringstream buff;
				buff << ((m_NineHid == 2) ? "Две" : "Три")
						<< " девятки в прикупе! ";
				if (gi.m_human)
				{
					buff << "\nПересдать?";
					retval = YesNo(buff.str());
				}
				else
					retval = gi.Analisys(buff.str());

				if (retval)
					break;
			}
		} //<--проверки в прикупе

		// проверки валидности карт на руках
		// четыре 9-ки
		if (gi.m_ForthNine == 4 && m_rules->m_opt.Respawn49)
		{
			ostringstream buff;
			buff << gi.m_name << ": Четыре девятки на руках! ";

			if (gi.m_human)
			{
				buff << "\nПересдать?";
				retval = YesNo(buff.str());
			}
			else
				retval = gi.Analisys(buff.str());

			if (retval)
				break;
		}

		// минимальное количество очков
		if (gi.m_SumPoints <= m_rules->m_opt.RespawnHands)
		{
			ostringstream buff;
			buff << gi.m_name << " Сумма очков на руках " << gi.m_SumPoints;
			if (gi.m_SumPoints == m_rules->m_opt.RespawnHands)
				buff << ", что равно минимальному ";
			else
				buff << " из минимальных ";
			buff << m_rules->m_opt.RespawnHands << "!";

			if (gi.m_human)
			{
				buff << "\nПересдать?";
				retval = YesNo(buff.str());
			}
			else
				retval = gi.Analisys(buff.str());

			if (retval)
				break;
		}
	}

	return retval;
}

void Engine::ShowHidden(void)
{
	cout << "Hidden Cards: \n";

	for (ushort i = 0; i < 3; ++i)
		cout << "┌───┐";
	cout << "\n";

	for (auto it : m_HiddenCards)
		it.ShowCard();
	cout << "\n";

	for (ushort i = 0; i < 3; ++i)
		cout << "└───┘";
	cout << endl;
}

bool Engine::Start(void)
{
	bool retval = false;

	if (m_Thousand >= 1000)
	{
		//added Win!
		retval = false;
	}
	else
	{
		cout << "───Con: " << ++m_StepCon
				<< "─────────────────────────────────────────────────────────────────────────────────────"
				<< endl;
		cout << "\'q\' exit, any key next con... \n";

		switch (getch())
		{
		case 'q':
			return false;
			break;
		default:
			SpawnCard();
			if (GamersStatus())
				break;

			Auction();
			if (GamersStatus())
				break;

			SpawnAfAuc();
			if (GamersStatus())
				break;

			CorrectRate();
			Game();
			break;
		}
		retval = true;
	}

	m_Thousand = 1000;

	return retval;
}
