/* ======================================================================
**  MainWindow.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _MainWindow_h_
#define _MainWindow_h_

#include <QtGui>

// ======================================================================
class MainWindow : public QMainWindow {
Q_OBJECT
private:
    QLabel* m_plblX;
    QLabel* m_plblY;

protected:
    virtual void mouseMoveEvent(QMouseEvent* pe)
    {
        m_plblX->setText("X=" + QString().setNum(pe->x()));
        m_plblY->setText("Y=" + QString().setNum(pe->y()));
    }

public:
    MainWindow(QWidget* pwgt = 0) : QMainWindow(pwgt) 
    {
       setMouseTracking(true);

       m_plblX = new QLabel(this);
       m_plblY = new QLabel(this);
       statusBar()->addWidget(m_plblY);
       statusBar()->addWidget(m_plblX);       
    }
};
#endif  //_MainWindow_h_
