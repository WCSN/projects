/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#include <QApplication>
#include "SDIProgram.h"

// ----------------------------------------------------------------------
int main(int argc, char** argv) 
{
    QApplication app(argc, argv);
    SDIProgram   psdi;

    psdi.setWindowTitle("SDI Program");
    psdi.resize(300, 300);
    psdi.show();

    return app.exec();
}
