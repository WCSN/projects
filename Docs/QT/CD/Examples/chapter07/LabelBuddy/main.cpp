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
    QLabel*    plblName = new QLabel("&Name:");
    QLineEdit* ptxtName = new QLineEdit;
    plblName->setBuddy(ptxtName);

    QLabel*    plblAge = new QLabel("&Age:");
    QSpinBox*  pspbAge = new QSpinBox;
    plblAge->setBuddy(pspbAge);

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(plblName);    
    pvbxLayout->addWidget(ptxtName); 
    pvbxLayout->addWidget(plblAge);
    pvbxLayout->addWidget(pspbAge);
    wgt.setLayout(pvbxLayout);

    wgt.show();    

    return app.exec();
}
