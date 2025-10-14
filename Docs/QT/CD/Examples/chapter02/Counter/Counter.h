/* ======================================================================
**  Counter.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _Counter_h_
#define _Counter_h_

#include <QObject>

// ======================================================================
class Counter : public QObject {
    Q_OBJECT
private:
    int m_nValue;

public:
    Counter();

public slots:
    void slotInc();

signals:
    void goodbye       (   );
    void counterChanged(int);
};
#endif  //_Counter_h_
