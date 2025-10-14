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
    QBoxLayout* pbxLayout = new QBoxLayout(QBoxLayout::LeftToRight);    
    pbxLayout->addWidget(pcmdA, 1);
    pbxLayout->addWidget(pcmdB, 2);
    pbxLayout->addWidget(pcmdC, 3);
    wgt.setLayout(pbxLayout);

    wgt.resize(450, 40);
    wgt.show();

    return app.exec();
}
