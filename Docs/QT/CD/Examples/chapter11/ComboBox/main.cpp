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
    QComboBox    cbo;
    QStringList  lst;

    lst << "John" << "Paul" << "George" << "Ringo";
    cbo.addItems(lst);
    cbo.setEditable(true);
    cbo.show();

    return app.exec();
}
