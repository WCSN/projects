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
    QTabWidget   tab;
    QStringList  lst;

    lst << "Linux" << "Windows" << "MacOS" << "OS2";
    foreach(QString str, lst) {
        tab.addTab(new QLabel(str, &tab), QPixmap(str + ".jpg"), str);
    }
                                                                
    tab.resize(300, 100);
    tab.show();

    return app.exec();
}
