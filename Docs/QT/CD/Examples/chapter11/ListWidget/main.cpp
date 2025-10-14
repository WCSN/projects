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
    QApplication     app(argc, argv);
    QStringList      lst;
    QListWidget      lwg;
    QListWidgetItem* pitem = 0;

    lwg.setIconSize(QSize(48, 48));
    lst << "Linux" << "Windows" << "MacOS" << "OS2";
    foreach(QString str, lst) {
        pitem = new QListWidgetItem(str, &lwg);    
        pitem->setIcon(QPixmap(str + ".jpg"));
    }
    lwg.resize(125, 175);
    lwg.show();

    return app.exec();
}
