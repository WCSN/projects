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

    QWidget    wgt;
    QCheckBox* pchkNormal = new QCheckBox("&Normal Check Box");
    pchkNormal->setChecked(true);

    QCheckBox* pchkTristate = new QCheckBox("&Tristate Check Box");
    pchkTristate->setTristate(true);
    pchkTristate->setCheckState(Qt::PartiallyChecked);

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(pchkNormal);    
    pvbxLayout->addWidget(pchkTristate);
    wgt.setLayout(pvbxLayout);

    wgt.show();

    return app.exec();
}
