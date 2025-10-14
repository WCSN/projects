#include "plotterbox.h"
#include "ui_plotterbox.h"

PlotterBox::PlotterBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotterBox)
{
    ui->setupUi(this);

    ui->splMngViewBox->setStretchFactor( 0, 1 );
    ui->splMngViewBox->setStretchFactor( 1, 6 );
    ui->splMngViewBox->setStretchFactor( 2, 3 );
}

PlotterBox::~PlotterBox()
{
    delete ui;
}
