/*
 * Card_.h
 *
 *  Created on: 06.06.2013
 *      Author: wocson
 */

#ifndef CARD_H_
#define CARD_H_

#include <sys/types.h>
#include <string>

using namespace std;

class Card
{
public:
	string  m_code;
	string  m_imagename;
	// ♥♡C/1 червы—hearts, ♦♢B/2 бубны — diamonds,♣♧K/3 трефы, крести—clubs, ♠♤V/4 пики,вини—spades
	ushort m_lear; // масть.
	ushort m_points;
	ushort m_numcod;

public:

	Card(const string code,const string imagename,ushort lear, ushort points, ushort m_numcod);
	void ShowCard(void)const;

	virtual ~Card();
};

#endif /* CARD_H_ */
