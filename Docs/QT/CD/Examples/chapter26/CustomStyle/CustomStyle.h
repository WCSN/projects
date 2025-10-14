/* ======================================================================
**  CustomStyle.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _CustomStyle_h_
#define _CustomStyle_h_

#include <QWindowsStyle>

class QPainter;

// ======================================================================
class CustomStyle : public QWindowsStyle {
public:
    virtual void polish  (QWidget* pwgt);
    virtual void unpolish(QWidget* pwgt);

    virtual void drawPrimitive(      PrimitiveElement elem, 
                               const QStyleOption*    popt,
                                     QPainter*        ppainter, 
                               const QWidget*         pwgt = 0
                              ) const;
};
#endif  //_CustomStyle_h_
