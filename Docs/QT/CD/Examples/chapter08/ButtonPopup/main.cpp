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
    QApplication app(argc, argv);

    QPushButton  cmd("Menu");
    QMenu*       pmnu = new QMenu(&cmd);
    pmnu->addAction("Item1");
    pmnu->addAction("Item2");
    pmnu->addAction("&Quit", &app, SLOT(quit()));

    cmd.setMenu(pmnu);
    cmd.show();

    return app.exec();
}
