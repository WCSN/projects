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
    QListWidget      lwg;
    QListWidgetItem* pitem = 0;
    QStringList      lst;

    lwg.setIconSize(QSize(48, 48));
    lwg.setSelectionMode(QAbstractItemView::MultiSelection);
    lwg.setViewMode(QListView::IconMode);
    lst << "Linux" << "Windows" << "MacOS" << "OS2";
    foreach(QString str, lst) {
        pitem = new QListWidgetItem(str, &lwg);    
        pitem->setIcon(QPixmap(str + ".jpg"));
        pitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | 
                        Qt::ItemIsEditable | Qt::ItemIsDragEnabled);

    }
    lwg.resize(135, 135);
    lwg.show();

    return app.exec();
}
