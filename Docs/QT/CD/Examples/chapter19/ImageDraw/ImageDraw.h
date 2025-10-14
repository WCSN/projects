/* ======================================================================
**  ImageDraw.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _ImageDraw_h_
#define _ImageDraw_h_

#include <QWidget>

// ======================================================================
class ImageDraw : public QWidget {
protected:
    virtual void paintEvent(QPaintEvent* pe);

public:
    ImageDraw(QWidget* pwgt = 0);
    virtual ~ImageDraw();
};

#endif  //_ImageDraw_h_
