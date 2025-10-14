/* ======================================================================
**  Drag.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _Drag_h_
#define _Drag_h_

#include <QtGui>
#include "img1.xpm"

// ======================================================================
class Drag : public QLabel {
Q_OBJECT
private:
    QPoint m_ptDragPos;

    void startDrag()
    {
        QMimeData* pMimeData = new QMimeData;
        pMimeData->setText(text());

        QDrag* pDrag = new QDrag(this);
        pDrag->setMimeData(pMimeData);
        pDrag->setPixmap(QPixmap(img1_xpm));
        pDrag->exec(Qt::MoveAction);
    }

protected:
    virtual void mousePressEvent(QMouseEvent* pe)
    {
        if (pe->button() == Qt::LeftButton) {
            m_ptDragPos = pe->pos();
        }
        QWidget::mousePressEvent(pe);
    }

    virtual void mouseMoveEvent (QMouseEvent* pe)
    {
        if (pe->buttons() & Qt::LeftButton) {
            int distance = (pe->pos() - m_ptDragPos).manhattanLength();
            if (distance > QApplication::startDragDistance()) {
                startDrag();
            }
        }
        QWidget::mouseMoveEvent(pe);
    }

public:
    Drag(QWidget* pwgt = 0) : QLabel("This is draggable text", pwgt)
    {
    }
};
#endif //_Drag_h_
