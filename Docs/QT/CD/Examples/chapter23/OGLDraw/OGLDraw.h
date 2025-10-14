/* ======================================================================
**  OGLDraw.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _OGLDraw_h_
#define _OGLDraw_h_

#include <QGLWidget>

// ======================================================================
class OGLDraw : public QGLWidget {
protected:
    virtual void initializeGL(                       );
    virtual void resizeGL    (int nWidth, int nHeight);
    virtual void paintGL     (                       );

public:
    OGLDraw(QWidget* pwgt = 0);

    void draw(int xOffset, int yOffset, GLenum type);
};
#endif  //_OGLDraw_h_
