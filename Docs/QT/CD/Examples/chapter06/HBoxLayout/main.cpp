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
    QWidget      wgt;

    QPushButton* pcmdA = new QPushButton("A");
    QPushButton* pcmdB = new QPushButton("B");
    QPushButton* pcmdC = new QPushButton("C");

    //Layout setup
    QHBoxLayout* phbxLayout = new QHBoxLayout;    
    phbxLayout->setMargin(10);
    phbxLayout->setSpacing(20);
    phbxLayout->addWidget(pcmdA);
    phbxLayout->addWidget(pcmdB);
    phbxLayout->addWidget(pcmdC);
    wgt.setLayout(phbxLayout);

    wgt.show();

    return app.exec();
}
