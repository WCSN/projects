/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/
#include <QtGui>

// ======================================================================
class MyLabel : public QLabel {
public:
    MyLabel(QWidget* pwgt = 0) : QLabel(pwgt)
    {
    }

    virtual void paintEvent(QPaintEvent*)
    {
        QStylePainter      painter(this);
        QStyleOptionButton option;

        option.initFrom(this);
        option.text = "This is a label";

        painter.drawControl(QStyle::CE_PushButton, option);
    }
};

// ----------------------------------------------------------------------
int main (int argc, char** argv)
{
    QApplication  app(argc, argv);  
    MyLabel       lbl;

    lbl.resize(100, 50);
    lbl.show();

    return app.exec();
}
