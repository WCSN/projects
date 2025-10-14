#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QAction>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle ("Print Document");
    setWindowIcon(QPixmap(":/new/prefix1/images/file.png"));

    QAction * action_print = new QAction (tr ("Print "), this);
    QAction * action_printpreview = new QAction ( tr("Print Preview "), this);
    QAction * action_pdf = new QAction ( tr("Generate PDF"), this);

    connect(action_print,SIGNAL(triggered()),this,SLOT(doPrint()));
    connect(action_printpreview,SIGNAL(triggered()),this,SLOT(doPrintPreview()));
    connect(action_pdf,SIGNAL(triggered()),this,SLOT(creatPdf()));

    ui->toolBar->addAction(action_print);
    ui->toolBar->addAction(action_printpreview);
    ui->toolBar->addAction(action_pdf);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doPrint()
{
    QPrinter Printer;

    QPrintDialog dlg (&Printer, this); // Create print dialog
    // If the editor is selected from the selected area, select the area

    if(ui->textEdit->textCursor().hasSelection())
        dlg.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if(dlg.exec() == QDialog::Accepted)
    {
        // If you press the print button in the dialog
        // execute print operation
        ui->textEdit->print(&Printer);
    }
}

//Printing preview
void MainWindow::doPrintPreview()
{
    QPrinter printer;
    QPrintPreviewDialog Preview (&printer, this);

    connect(&Preview,&QPrintPreviewDialog::paintRequested,this,&MainWindow::printPreview);

    Preview.exec();
}

void MainWindow::printPreview(QPrinter *printer)
{
    //vplot->Printer(printer);

/*
    printer->setPageSize(QPrinter::Letter);
    printer->setOrientation(QPrinter::Landscape);
    printer->setPageMargins(0.75, 0.75, 0.75, 0.75, QPrinter::Unit::Inch);
    QCPPainter painter(printer);
    QRectF pageRect = printer->pageRect(QPrinter::DevicePixel);

    // set size on page
    int plotWidth = ui.plotWidget->viewport().width();
    int plotHeight = ui.plotWidget->viewport().height();

    double scale = pageRect.width() / (double)plotWidth;
    double scale2 = pageRect.height() / (double)plotHeight;

    if (scale2 < scale)
        scale = scale2;

    painter.setMode(QCPPainter::pmVectorized);
    painter.setMode(QCPPainter::pmNoCaching);
    painter.setMode(QCPPainter::pmNonCosmetic);

    painter.scale(scale, scale);
    ui.plotWidget->toPainter(&painter, plotWidth, plotHeight);
    painter.drawText(-10, -10, QDateTime::currentDateTime().toString());
*/
}

void MainWindow::createPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF File"), QString (), "*. PDF");

    if(!fileName.isEmpty())
    {
        if(QFileInfo(fileName).suffix().isEmpty())
            fileName.append(". pdf"); // If the file suffix is ​​empty, the default uses .pdf

        QPrinter printer;
        printer.setOutputFormat (QPrinter::PdfFormat); // Specify the output format to PDF
        printer.setOutputFileName(fileName);
        ui->textEdit->print(&printer);
    }
}

void MainWindow::on_pushButton_released()
{

}
