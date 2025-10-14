/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <QtGui>
#include "HelpBrowser.h"

// ----------------------------------------------------------------------
int main (int argc, char** argv)
{
    QApplication app(argc, argv);  
    HelpBrowser  helpBrowser("doc", "index.htm");

    helpBrowser.resize(400, 300);
    helpBrowser.show();

    return app.exec();
}
