/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include "CustomWidget.h"

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget      wgt;

    CustomWidget* pcw  = new CustomWidget;
    QScrollBar*   phsb = new QScrollBar(Qt::Horizontal);

    phsb->setRange(0, 100); 

    QObject::connect(phsb, SIGNAL(valueChanged(int)), 
                     pcw, SLOT(slotSetProgress(int))
                    );

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;    
    pvbxLayout->addWidget(pcw);
    pvbxLayout->addWidget(phsb);
    wgt.setLayout(pvbxLayout);

    wgt.show();

    return app.exec();
}
