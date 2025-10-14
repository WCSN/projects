/* ======================================================================
**  OGLPyramid.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _OGLPyramid_h_
#define _OGLPyramid_h_

#include <QGLWidget>

// ======================================================================
class OGLPyramid : public QGLWidget {
private:
    GLuint  m_nPyramid;
    GLfloat m_xRotate;
    GLfloat m_yRotate;
    QPoint  m_ptPosition;

protected:
    virtual void   initializeGL   (                       );
    virtual void   resizeGL       (int nWidth, int nHeight);
    virtual void   paintGL        (                       );
    virtual void   mousePressEvent(QMouseEvent* pe        );
    virtual void   mouseMoveEvent (QMouseEvent* pe        );
            GLuint createPyramid  (GLfloat fSize = 1.0f   );

public:
    OGLPyramid(QWidget* pwgt = 0);
};
#endif  //_OGLPyramid_h_
