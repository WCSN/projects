/* ======================================================================
**  GrabWidget.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#ifndef _GrabWidget_h_
#define _GrabWidget_h_

#include <QWidget>

class QLabel;

// ======================================================================
class GrabWidget : public QWidget {
Q_OBJECT
private:
     QLabel* m_plbl;

public:
    GrabWidget(QWidget* pwgt = 0);

public slots:
    void slotGrabScreen();
};
#endif  //_GrabWidget_h_
