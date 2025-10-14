/* ======================================================================
**  Widget.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/
#ifndef _Widget_h_
#define _Widget_h_

#include <QPoint>
#include <QLabel>

// ======================================================================
class Widget : public QLabel {
Q_OBJECT
private:
    QPoint m_ptDragPos;

    void startDrag();

protected:
    virtual void mousePressEvent(QMouseEvent*    );
    virtual void mouseMoveEvent (QMouseEvent*    );
    virtual void dragEnterEvent (QDragEnterEvent*);
    virtual void dropEvent      (QDropEvent*     );

public:
    Widget(QWidget* pwgt = 0);
};
#endif //_Widget_h_
