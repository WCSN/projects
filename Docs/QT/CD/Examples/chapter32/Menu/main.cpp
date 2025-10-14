/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include "img4.xpm"

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QMenuBar mnuBar;
    QMenu*   pmnu   = new QMenu("&Menu");

    pmnu->addAction("&About Qt", 
                    &app, 
                    SLOT(aboutQt()), 
                    Qt::CTRL + Qt::Key_Q
                   );

    pmnu->addSeparator();

    QAction* pCheckableAction = pmnu->addAction("&CheckableItem");
    pCheckableAction->setCheckable(true);
    pCheckableAction->setChecked(true);

    pmnu->addAction(QPixmap(img4_xpm), "&IconItem");

    QMenu* pmnuSubMenu = new QMenu("&SubMenu", pmnu);
    pmnu->addMenu(pmnuSubMenu);
    pmnuSubMenu->addAction("&Test");

    QAction* pDisabledAction = pmnu->addAction("&DisabledItem");
    pDisabledAction->setEnabled(false);

    pmnu->addSeparator();

    pmnu->addAction("&Exit", &app, SLOT(quit()));

    mnuBar.addMenu(pmnu);
    mnuBar.show();

    return app.exec();
}
