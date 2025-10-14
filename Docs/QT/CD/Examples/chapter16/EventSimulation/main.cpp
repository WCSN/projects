/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#include <QtGui>

// ----------------------------------------------------------------------
int main (int argc, char** argv)
{
    QApplication app (argc, argv);

    QLineEdit txt("User input: ");
    txt.show();
    txt.resize(280, 20);

    int i;
    for (i = 0; i < Qt::Key_Z - Qt::Key_A + 1; ++i) {
        QChar ch      = 65 + i;
        int   nKey    = Qt::Key_A + i;
        QKeyEvent* pe = 
            new QKeyEvent(QEvent::KeyPress, nKey, Qt::NoModifier, ch);
        QApplication::sendEvent(&txt, pe);
    }

    return app.exec();
}

