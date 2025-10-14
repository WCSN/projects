/* ======================================================================
**  MyView.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _MyView_h_
#define _MyView_h_

#include <QGraphicsView>

// ======================================================================
class MyView: public QGraphicsView {
Q_OBJECT
public:
    MyView(QGraphicsScene* pScene, QWidget* pwgt = 0) 
        : QGraphicsView(pScene, pwgt)
    {
    }

public slots:
    void slotZoomIn()
    {
        scale(1.1, 1.1);
    }

    void slotZoomOut()
    {
        scale(1 / 1.1, 1 / 1.1);
    }

    void slotRotateLeft()
    {
        rotate(-5);
    }

    void slotRotateRight()
    {
        rotate(5);
    }
};
#endif  //_MyView_h_
