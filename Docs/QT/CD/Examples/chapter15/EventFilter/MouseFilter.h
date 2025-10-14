/* ======================================================================
**  MouseFilter.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _MouseFilter_h_
#define _MouseFilter_h_

#include <QObject>

// ======================================================================
class MouseFilter : public QObject {
protected:
    virtual bool eventFilter(QObject*, QEvent*);

public:
    MouseFilter(QObject* pobj = 0);

};

#endif  //_MouseFilter_h_
