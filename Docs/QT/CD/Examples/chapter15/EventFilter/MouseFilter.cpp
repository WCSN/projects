/* ======================================================================
**  MouseFilter.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <QtGui>
#include "MouseFilter.h"

// ----------------------------------------------------------------------
MouseFilter::MouseFilter(QObject* pobj/*= 0*/) 
    : QObject(pobj)
{
}

// ----------------------------------------------------------------------
/*virtual*/bool MouseFilter::eventFilter(QObject* pobj, QEvent* pe)
{
    if (pe->type() == QEvent::MouseButtonPress) {
        if (((QMouseEvent*)pe)->button() == Qt::LeftButton) {
            QString strClassName = pobj->metaObject()->className();
            QMessageBox::information(0, "Class Name", strClassName);
            return true;
        }
    }
    return false;
}
