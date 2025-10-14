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
    QLabel       lbl("this is the example text");
    QLibrary     lib("dynlib");

    typedef QString (*Fct) (const QString&);
    Fct fct = (Fct)(lib.resolve("oddUpper"));
    if (fct) {
        lbl.setText(fct(lbl.text()));
    }
    lbl.show();

    return app.exec();
}
