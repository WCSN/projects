/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <QApplication>
#include "StartDialog.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    StartDialog startDialog;
    
    startDialog.setMinimumWidth(200);
    startDialog.show();

    return app.exec();
}

