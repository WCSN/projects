/* ======================================================================
**  MainWindow.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _MainWindow_h_
#define _MainWindow_h_

#include <QtGui>
#include "img1.xpm"
#include "img2.xpm"
#include "img3.xpm"
#include "img4.xpm"

// ======================================================================
class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget* pwgt = 0) : QMainWindow(pwgt) 
    {
        addToolBar(Qt::TopToolBarArea, createToolBar());
        addToolBar(Qt::BottomToolBarArea, createToolBar());
        addToolBar(Qt::LeftToolBarArea, createToolBar());
        addToolBar(Qt::RightToolBarArea, createToolBar());
    }

    QToolBar* createToolBar()
    {
        QToolBar* ptb = new QToolBar("Linker ToolBar");

        ptb->addAction(QPixmap(img1_xpm), "1", this, SLOT(slotNoImpl()));
        ptb->addAction(QPixmap(img2_xpm), "2", this, SLOT(slotNoImpl()));
        ptb->addSeparator();
        ptb->addAction(QPixmap(img3_xpm), "3", this, SLOT(slotNoImpl()));
        ptb->addAction(QPixmap(img4_xpm), "4", this, SLOT(slotNoImpl()));

        return ptb;
    }

public slots:
    void slotNoImpl()
    {
        QMessageBox::information(0, "Message", "Not implemented");
    }
};
#endif  //_MainWindow_h_
