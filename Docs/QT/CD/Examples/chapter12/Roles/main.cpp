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
    QStringList  lst;
    lst << "Linux" << "Windows" << "MacOS" << "OS2";

    QStandardItemModel model(lst.size(), 1);
    for (int i = 0; i < model.rowCount(); ++i) {
        QModelIndex index = model.index(i, 0);
        QString     str   = lst.at(i);
        model.setData(index, str, Qt::DisplayRole);
        model.setData(index, "ToolTip for " + str, Qt::ToolTipRole);
        model.setData(index, QIcon(str + ".jpg"), Qt::DecorationRole);
    }

    QListView listView;
    listView.setViewMode(QListView::IconMode);
    listView.setModel(&model);
    listView.show();

    return app.exec();
}
