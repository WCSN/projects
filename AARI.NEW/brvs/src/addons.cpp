/*
 * addons.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: wocson
 */

#include <stdlib.h>
#include <stdio.h>
#include "addons.h"

WLabel::WLabel(const QString& strText, QWidget* parent, const QColor &tcolor, const QColor &bcolor, int nInterval) :
		QLabel(strText, parent), m_bBlink(true), m_strText(strText)
{
	QPalette pal = palette();
	if( tcolor != -1 )
		pal.setColor(QPalette::WindowText, tcolor);
	if( bcolor != -1 )
	{
		pal.setColor(QPalette::Background, bcolor);
		setAutoFillBackground(true);
	}
	setPalette(pal);

	if( nInterval > 0 ) startTimer(nInterval);
}

void WLabel::timerEvent(QTimerEvent*)
{
	m_bBlink = !m_bBlink;
	setText(m_bBlink ? m_strText : "");
}

double randomW(int first, int last)
{
    return (double) (rand() % (last - first) + first);

    /*
     v1 = rand() % 100;         // v1 in the range 0 to 99
     v2 = rand() % 100 + 1;     // v2 in the range 1 to 100
     v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014
     */
}
