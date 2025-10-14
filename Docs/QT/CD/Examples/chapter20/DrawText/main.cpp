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
class DrawTextWidget : public QWidget {
protected:
    virtual void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        QRect    r = rect();
        painter.setFont(QFont("Times", 50, QFont::Normal));
        painter.drawRect(r); 
        painter.drawText(r, Qt::AlignCenter | Qt::TextWordWrap, "Draw Text");
    }

public:
    DrawTextWidget(QWidget* pwgt = 0) : QWidget(pwgt)
    {
    }
};


// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    DrawTextWidget dtw;

    dtw.resize(200, 150);
    dtw.show();

    return app.exec();
}
