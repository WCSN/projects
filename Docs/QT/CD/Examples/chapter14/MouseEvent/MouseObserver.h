/* ======================================================================
**  MouseObserver.h --
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _MouseObserver_h_
#define _MouseObserver_h_

#include <QtGui>

// ======================================================================
class MouseObserver : public QLabel {
public:
    MouseObserver(QWidget* pwgt = 0); 

protected:
    virtual void mousePressEvent  (QMouseEvent* pe);
    virtual void mouseReleaseEvent(QMouseEvent* pe);
    virtual void mouseMoveEvent   (QMouseEvent* pe);

    void    dumpEvent     (QMouseEvent* pe, const QString& strMessage);
    QString modifiersInfo (QMouseEvent* pe                           );
    QString buttonsInfo   (QMouseEvent* pe                           );
};

#endif //_MouseObserver_h_
