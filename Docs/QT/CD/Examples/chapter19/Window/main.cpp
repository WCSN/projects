/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#include <QtGui>

// ======================================================================
class Window : public QLabel {
private:
    QPoint  m_ptPosition;

protected:
    virtual void Window::mousePressEvent(QMouseEvent* pe)
    {
        m_ptPosition = pe->pos();
    }

    virtual void Window::mouseMoveEvent(QMouseEvent* pe)
    {
        move(pe->globalPos() - m_ptPosition);
    }

public:
    Window(QWidget* pwgt = 0) 
        : QLabel(pwgt,  Qt::FramelessWindowHint | Qt::Window) 
    {
    }
};

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Window       win;
    QPixmap      pix(":/images/unixoids.png");

    win.setPixmap(pix);
    win.setMask(pix.mask());
    win.show();

    return app.exec();
}
