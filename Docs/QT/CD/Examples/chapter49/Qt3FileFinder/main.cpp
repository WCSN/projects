/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2004 by Max Schlee
** ======================================================================
*/
#include <qapplication.h>
#include "FileFinder.h"

// ----------------------------------------------------------------------
int main (int argc, char** argv)
{
    QApplication app(argc, argv);  

    FileFinder fileFinder;

    app.setMainWidget(&fileFinder);

    fileFinder.resize(370, 240);
    fileFinder.show();

    return app.exec();
}
