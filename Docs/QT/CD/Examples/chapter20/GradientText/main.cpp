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
        QLinearGradient gradient(0, 0, 500, 0);
        gradient.setColorAt(0, Qt::red);
        gradient.setColorAt(0.5, Qt::green);
        gradient.setColorAt(1, Qt::blue);

        QPainter painter(this);
        painter.setPen(QPen(gradient, 0));
        painter.setFont(QFont("Times", 50, QFont::Normal));
        painter.drawText(60, 60, "Gradient Text");
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

    dtw.resize(500, 80);
    dtw.show();

    return app.exec();
}
