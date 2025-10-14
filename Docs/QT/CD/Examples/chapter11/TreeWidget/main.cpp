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
    QTreeWidget  twg;
    QStringList  lst;

    lst << "Folders" << "Used Space";
    twg.setHeaderLabels(lst);
    twg.setSortingEnabled(true);

    QTreeWidgetItem* ptwgItem = new QTreeWidgetItem(&twg);
    ptwgItem->setText(0, "Local Disk(C)");
    ptwgItem->setIcon(0, QPixmap("drive.bmp"));

    QTreeWidgetItem* ptwgItemDir = 0;
    for (int i = 1; i < 20; ++i) {
        ptwgItemDir = new QTreeWidgetItem(ptwgItem);
        ptwgItemDir->setText(0, "Directory" + QString::number(i));
        ptwgItemDir->setText(1, QString::number(i) + "MB");
        ptwgItemDir->setIcon(0, QPixmap("folder.bmp"));
    }
    twg.setItemExpanded(ptwgItem, true);
    twg.resize(250, 110);
    twg.show();

    return app.exec();
}
