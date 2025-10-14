/* ======================================================================
**  OGLQuad.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _OGLQuad_h_
#define _OGLQuad_h_

#include <QGLWidget>

// ======================================================================
class OGLQuad : public QGLWidget {
protected:
    virtual void initializeGL(                       );
    virtual void resizeGL    (int nWidth, int nHeight);
    virtual void paintGL     (                       );

public:
    OGLQuad(QWidget* pwgt = 0);
};
#endif  //_OGLQuad_h_
