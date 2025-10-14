/* ======================================================================
**  Widget.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/
#include <QtGui>
#include "Widget.h"
#include "WidgetDrag.h"

// ----------------------------------------------------------------------
Widget::Widget(QWidget* pwgt/*=0*/) : QLabel(pwgt)
{
    setAcceptDrops(true);
}

// ----------------------------------------------------------------------
void Widget::startDrag()
{
    WidgetDrag* pDrag = new WidgetDrag(this);
    pDrag->setWidget(this);
    pDrag->exec(Qt::CopyAction);
}

// ----------------------------------------------------------------------
/*virtual*/ void Widget::mousePressEvent(QMouseEvent* pe)
{
    if (pe->button() == Qt::LeftButton) {
        m_ptDragPos = pe->pos();
    }
    QWidget::mousePressEvent(pe);
}

// ----------------------------------------------------------------------
/*virtual*/ void Widget::mouseMoveEvent(QMouseEvent* pe)
{
    if (pe->buttons() & Qt::LeftButton) {
        int distance = (pe->pos() - m_ptDragPos).manhattanLength();
        if (distance > QApplication::startDragDistance()) {
            startDrag();
        }
    }
    QWidget::mouseMoveEvent(pe);
}

// ----------------------------------------------------------------------
/*virtual*/ void Widget::dragEnterEvent(QDragEnterEvent* pe)
{
    if (pe->mimeData()->hasFormat(WidgetMimeData::mimeType())) {
        pe->acceptProposedAction();
    }
}

// ----------------------------------------------------------------------
/*virtual*/ void Widget::dropEvent(QDropEvent* pe)
{
    const WidgetMimeData* pmmd = 
        dynamic_cast<const WidgetMimeData*>(pe->mimeData());
    if (pmmd) {
        QWidget* pwgt = pmmd->widget();
        QString str("Widget is dropped\n ObjectName:%1");
        setText(str.arg(pwgt->objectName()));
    }
}

