/*
 * Card_.cpp
 *
 *  Created on: 06.06.2013
 *      Author: wocson
 */

#include <iostream>

#include "Card.h"

using namespace std;

Card::Card(string code,string imagename,u_short lear,u_short points, ushort numcod):
m_code(code), m_imagename(imagename),m_lear(lear),m_points(points),m_numcod(numcod)
{

}

Card::~Card()
{

}

void Card::ShowCard(void)const
{
	cout << "│"<< m_imagename << m_code <<"│";
}
