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
    QToolBox     tbx;
    QStringList  lst;

    lst << "Linux" << "Windows" << "MacOS" << "OS2";
    foreach(QString str, lst) {
        tbx.addItem(new QLabel(str, &tbx), QPixmap(str + ".jpg"), str);
    }
    tbx.resize(100, 80);
    tbx.show();

    return app.exec();
}
