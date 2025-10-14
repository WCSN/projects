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
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QMenuBar pmnuBar;
    QMenu*   pmnu    = new QMenu("&Menu");

    pmnu->setTearOffEnabled(true);

    pmnu->addAction("Item&1");
    pmnu->addAction("Item&2");
    pmnu->addAction("Item&3");
    pmnu->addAction("&Exit", &app, SLOT(quit()));

    pmnuBar.addMenu(pmnu);
    pmnuBar.show();

    return app.exec();
}
