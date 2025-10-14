/* ======================================================================
**  GrabWidget.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include "GrabWidget.h"

// ----------------------------------------------------------------------
GrabWidget::GrabWidget(QWidget* pwgt /*=0*/) : QWidget(pwgt)
{
    resize(640, 480);

    m_plbl = new QLabel();

    QPushButton* pcmd = new QPushButton("Capture Screen");
    connect(pcmd, SIGNAL(clicked()), SLOT(slotGrabScreen()));

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;    
    pvbxLayout->addWidget(pcmd);
    pvbxLayout->addWidget(m_plbl);
    setLayout(pvbxLayout);
}

// ----------------------------------------------------------------------
void GrabWidget::slotGrabScreen()
{
    QDesktopWidget* pwgt = QApplication::desktop();
    QPixmap         pic  = QPixmap::grabWindow(pwgt->screen()->winId());

    m_plbl->setPixmap(pic.scaled(m_plbl->size()));
}
