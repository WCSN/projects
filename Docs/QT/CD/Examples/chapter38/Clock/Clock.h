/* ======================================================================
**  Clock.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _Clock_h_
#define _Clock_h_

#include <QtGui>

// ======================================================================
class Clock : public QLabel {
    Q_OBJECT

public:
    Clock(QWidget* pwgt = 0) : QLabel(pwgt)
    {
        QTimer* ptimer = new QTimer(this);
        connect(ptimer, SIGNAL(timeout()), SLOT(slotUpdateDateTime()));
        ptimer->start(500);
        slotUpdateDateTime();
    }

public slots:
    void slotUpdateDateTime()
    {
        QString str = 
            QDateTime::currentDateTime().toString(Qt::SystemLocaleDate);
        setText("<H2><CENTER>" + str + "</CENTER></H2>");
    }
};
#endif  //_Clock_h_
