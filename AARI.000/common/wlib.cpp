/*
 * addons.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: wocson
 */
#include "wlib.h"

WLabel::WLabel(const QString& strText, QWidget* parent, const QColor &tcolor, const QColor &bcolor, int nInterval) :
        QLabel(strText, parent), m_bBlink(true), m_strText(" "+strText)
{
    wsetColors(tcolor, bcolor);
    setAlignment(Qt::AlignCenter);
    QFont fnt("Mono"); fnt.setBold(true);
    fnt.setPointSize(10);
    setFont(fnt);

    if( nInterval > 0 ) startTimer(nInterval);
}

void WLabel::wsetColors(const QColor &tcolor, const QColor &bcolor)
{
    QPalette pal = palette();
    if( tcolor != -1 )
        pal.setColor(QPalette::WindowText, tcolor);
    if( bcolor != -1 )
    {
        pal.setColor(QPalette::Window, bcolor);
        setAutoFillBackground(true);
    }
    setPalette(pal);
}

void WLabel::wsetText(const QString &text)
{
    m_strText = text;
    if( nInterval == 0 ) setText(text);
}

void WLabel::timerEvent(QTimerEvent*)
{
    m_bBlink = !m_bBlink;
    setText(m_bBlink ? m_strText : "");
}

double randomW(int32_t first, int32_t last)
{
    return (double) (rand() % (last - first) + first);

    /*
     v1 = rand() % 100;         // v1 in the range 0 to 99
     v2 = rand() % 100 + 1;     // v2 in the range 1 to 100
     v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014
     */
}

QString CurrentTime(QString tmformat)
{
    if (tmformat.isEmpty())
        tmformat.push_back("yyyyMMdd-hhmmss");

    QDateTime tm = QDateTime::currentDateTime();
    return tm.toString(tmformat);
}
