/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <QApplication>
#include "TurtleWorkArea.h"

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    TurtleWorkArea turtleWorkArea;
    turtleWorkArea.show();

    return app.exec();
}
