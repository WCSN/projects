#include <QLayout>
#include <QLabel>
#include <QFrame>
#include <QSplitter>
#include <QTextEdit>
#include <QDateTime>
#include <QTextStream>
#include <QWheelEvent>
#include <QPalette>
#include <QPrinter>

#include "options.h"
#include "mainwindow.h"
#include "wlib.h"

#include "ui_mainwindow.h"
#include "ui_options.h"
#include "ui_plotterbox.h"

#include "vplot.h"
#include "plotterbox.h"

//
//  Не забудь, что почистил всю многографиксность.
//
VPlot::VPlot(QWidget *parent, MainWindow *MW) :
    QWidget( parent ),
    mw(MW),
    fontVPlot(mw->mainFont),
    m_rangeX0(mw->defpar.AxisX0),
    m_rangeXS(mw->defpar.AxisX0Size),
    m_minVG0(mw->defpar.AxisYmin),
    m_maxVG0(mw->defpar.AxisYmax)
{
    mw->ui->layoutGridPlot->setMargin(3);
    mw->ui->layoutGridPlot->setSpacing(3);

    splPlot = new QSplitter( Qt::Horizontal );

    mngViewBox = new PlotterBox(this);

    mPlot = new QCustomPlot(this);
    CreatePlotter();

    splPlot->addWidget( mPlot );
    splPlot->addWidget( mngViewBox );

    splPlot->setStretchFactor( 0, 32 );
    splPlot->setStretchFactor( 1, 1 );

    mw->ui->layoutGridPlot->addWidget( splPlot );

    //textItem = new QCPItemText(mPlot);
    msMv = new QLabel(mPlot);
    msMv->setFont(*fontVPlot);
    msWl = new QLabel(mPlot);
    msWl->setFont(*fontVPlot);

    connect(axisYL0, SIGNAL(rangeChanged(QCPRange)), axisYR0, SLOT(setRange(QCPRange)));
    connect(axisYR0, SIGNAL(rangeChanged(QCPRange)), axisYL0, SLOT(setRange(QCPRange)));
    connect(mPlot, &QCustomPlot::mouseMove, this, &VPlot::on_MouseMove);
    connect(mPlot, &QCustomPlot::mouseWheel, this, &VPlot::on_MouseWheel);
    connect(mPlot, &QCustomPlot::mousePress, this, &VPlot::on_MousePress);
    connect(mPlot, &QCustomPlot::mouseRelease, this, &VPlot::on_MouseRelease);

    connect(mngViewBox->ui->pushBSavePlot, SIGNAL(clicked()), this, SLOT(on_pushBSavePlot_release()));
    connect(mngViewBox->ui->pushBPrint,SIGNAL(clicked()),this,SLOT(doPrintPreview()));
}

VPlot::~VPlot()
{
    closeFileSave();
}

void VPlot::initSettings(bool showOGL)
{
    m_rangeX0 = mw->ui->lineEditAxX0->text().toDouble();
    m_rangeXS = mw->ui->lineEditAxXsize->text().toDouble();
    m_rangeX1 = m_rangeXS;

    m_minVG0 = mw->ui->lineEditAxYmin->text().toDouble();
    m_maxVG0 = mw->ui->lineEditAxYmax->text().toDouble();

    filedatasave.clear();
    filedatasave.push_back(mw->curset.namedatafile);

    if(showOGL)
        setOpenGL(mw->opt->uiopt->checkBOGL->isChecked());
}

void VPlot::createFileSave(bool fullclean)
{
    if (mw->opt->uiopt->checkBAutoNameFile->isChecked() && fullclean)
    {
        mw->curset.namedatafile = QString("brga-" + CurrentTime("yyyyMMdd-hhmmss") + ".csv");
        mw->ui->lineEditFileDataSave->setText(mw->curset.namedatafile);
    }

    filedatasave = mw->curset.namedatafile;
    messageLog("Save data: " + mw->curset.pathdata);
    messageLog(filedatasave);

    if (fileData == nullptr)
    {
        if (!QDir(mw->curset.pathdata).exists())
            if (!QDir().mkpath(mw->curset.pathdata))
                messageLog("Path: " + mw->curset.pathdata + "not created");

        fileData = new QFile(mw->curset.pathdata+"/"+filedatasave);

        if (fileData->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            streamData = new QTextStream(fileData);
            streamData->setCodec("UTF-8");
            *streamData << tr("#N;Time;TimeExp;Data;") << Qt::endl;
            streamData->flush();
        }
        else
        {
            messageLog("File for data save not open");
        }
    }
}

void VPlot::closeFileSave(void)
{
    if (fileData != nullptr)
        if (fileData->isOpen())
            fileData->close();

    fileData = nullptr;
}

void VPlot::setaxisGrid(QCPAxis *axis)
{
    // Colors
    axis->setBasePen(QPen(Qt::black, 1));        // Горизонтальные линии оси координат
    axis->setTickPen(QPen(Qt::black, 1));        // Цвет маленькой галочки над числом на оси
    axis->setSubTickPen(QPen(Qt::black, 1));     // Цвет мелкой шкалы и конечной стрелки на оси
    axis->setTickLabelColor(QColor(0, 0, 127));  // Числа на осях
    axis->grid()->setPen(QPen(QColor(80, 80, 80), 1, Qt::DashLine)); // Формат рисования пунктирной линии по оси X номер (крупный масштаб)
    axis->grid()->setSubGridPen(QPen(QColor(127, 127, 127), 1, Qt::DotLine)); // Расширение формата рисования линии там, где нет числа на оси X (мелкий масштаб)
    axis->grid()->setSubGridVisible(true);       // Есть ли на графике пунктирная линия сетки
    axis->grid()->setZeroLinePen(Qt::SolidLine); // Устанавливаем перо нулевой линии, то есть вертикальную линию на 0 по оси X
}

void VPlot::setAxis_X(QCPAxis *axis)
{
    axis->setVisible(true);
    axis->setSubTicks(true);

    setaxisGrid(axis);
}

void VPlot::setAxis_Y(QCPAxis *axis)
{
    axis->setVisible(true);
    axis->setTickLabels(true);
    axis->setSubTicks(true);
    axis->setTickLabelRotation(0);
    axis->ticker()->setTickCount(10);
    axis->setNumberFormat("eb");
    axis->setNumberPrecision(2);
    axis->setRange(m_minVG0, m_maxVG0);

    setaxisGrid(axis);
}

void VPlot::CreatePlotter( void )
{
    //initSettings();

    mPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                           QCP::iSelectLegend | QCP::iSelectPlottables);

    mPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);   // Включаем перетаскивание
    mPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);   // Включаем удаление/приближение

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(230, 245, 230));
    plotGradient.setColorAt(1, QColor(230, 245, 230));
    mPlot->setBackground (plotGradient); // Устанавливаем цвет фона

    fontVPlot->setStyleStrategy(QFont::PreferAntialias);
    mPlot->setFont(*fontVPlot);

    // AXIS X
    //mPlot->axisRect()->addAxis(QCPAxis::atBottom);
    axisXB0 = mPlot->axisRect()->axis(QCPAxis::atBottom,0);
    //QSharedPointer<QCPAxisTickerDateTime> aXtmTick(new QCPAxisTickerDateTime);
    //aXtmTick->setTimeZone(QTimeZone::utc());
    //aXtmTick->setDateTimeFormat("hh:mm:ss");
    //axisXB0->setTicker(aXtmTick);

    //QSharedPointer<QCPAxisTickerFixed> fixTicker(new QCPAxisTickerFixed);
    //axisXB0->setTicker(fixTicker);

    //axisXB1 = mPlot->axisRect()->axis(QCPAxis::atBottom,1);

    setAxis_X(axisXB0); // axisX bottom X0
    //axisXB0->setNumberPrecision(0);
    axisXB0->setRange(m_rangeX0, m_rangeX1);

    //setAxis_X(axisXB1); // axisX bottom X1
    //axisXB1->setTicker(aXtmTick);

    axisXT0 = mPlot->axisRect()->axis(QCPAxis::atTop,0);
    axisXT0->setVisible( true );
    //axisXT0->setTicker(aXtmTick);
    axisXT0->setTicks(false);
    axisXT0->setSubTicks(false);

    // AXIS Y
    axisYL0 = mPlot->axisRect()->axis(QCPAxis::atLeft,0);
    setAxis_Y(axisYL0);

    axisYR0 = mPlot->axisRect()->axis(QCPAxis::atRight,0);
    setAxis_Y(axisYR0);

    mPlot->axisRect()->axis( QCPAxis::atRight, 0)->setPadding( 15 ); // add some padding to have space for tags
    mGraph0 = mPlot->addGraph( mPlot->xAxis, mPlot->axisRect()->axis( QCPAxis::atLeft, 0 ) );

    QPen penG0; penG0.setWidth(1);
    penG0.setColor(QColor( 0xAA, 0x00, 0x00 ));
    mGraph0->setPen( penG0 );

    mTag0 = new AxisTag(mGraph0->valueAxis()); // set tags with newly introduced AxisTag class (see axistag.h/.cpp)
    mTag0->setPen(penG0);
    mTag0->setText(QString().setNum(m_minVG0,'e',2));
    mTag0->setFont(*fontVPlot);
}

void VPlot::setSavePlotBoxEnabled(void)
{
    mngViewBox->ui->groupPlotBox->setEnabled(true);
}

void VPlot::Datafilter(QByteArray* buffer)
{
    QByteArray buf = *buffer;

    buf.replace(0x00, 0x20);
    buf.replace(0x0D, 0x20);
    buf.replace(0x0A, 0x23);

    mData.clear();
    mData = tr(buf);
    mData = mData.remove(QRegExp("[^0-9,.-+eE#]"));

}

void VPlot::setDataPlot(QByteArray *buffer)
{
    qDebug() << "A: " << *buffer;

    Datafilter(buffer);

    qDebug() << "B: " << mData;

    mw->loggedDmp(buffer);
    mw->loggedDat(&mData);

    QStringList buffValY0 = mData.split(0x23);
    QStringList buffValY1;

    for (int i = 0; i < buffValY0.size(); ++i)
        if (!buffValY0[i].isEmpty())
        {
            buffValY0[i] = buffValY0[i].simplified();
            buffValY1.append(buffValY0[i]);
        }

    buffValY0.clear();

    for (int i = 0; i < buffValY1.size(); ++i)
    {
        buffValY0 = buffValY1[i].split(0x2C);  // ',' 0x2C

        if (buffValY0.size() >= 2 && buffValY0[0] == QString("0"))
            mPlotUpdate(buffValY0[1].toDouble());
        else
            if(false)
            messageLog("Data error. Drop value.");

        qDebug() << "C: " << buffValY1[i];
    }
}

void VPlot::mPlotUpdate( double yV )
{
    // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
    if ( m_rangeX1 < mGraph0->dataCount())
    {
        m_rangeX0 = m_rangeX1;
        m_rangeX1 += m_rangeXS-1;
        axisXB0->setRange( m_rangeX0, m_rangeX1); //, Qt::AlignLeft );
    }

    mGraph0->addData(mGraph0->dataCount(), yV);
    mGraph0->rescaleValueAxis(true);

    const uint datacount = mGraph0->dataCount() - 1;

    double graph0V = mGraph0->dataMainValue( datacount );
    mTag0->updatePosition(graph0V);
    mTag0->setText( QString().setNum(graph0V, 'e', 2) );

    QDateTime tm = QDateTime::currentDateTime();
    mngViewBox->ui->mDataLog->insertPlainText(tm.toString("hh:mm:ss") + " - [ " + QString().setNum(datacount) + " ; " + mTag0->text() + " ]\n");
    mngViewBox->ui->mDataLog->verticalScrollBar()->setValue(mngViewBox->ui->mDataLog->verticalScrollBar()->maximum());
    datatocvs(tm.toString("hh:mm:ss") + ";" + QString().setNum(datacount) + ";" + mTag0->text() + ";");

    mPlot->replot();
}

void VPlot::datatocvs(const QString &outtext)
{
   *streamData << outtext << Qt::endl;
    streamData->flush();
}

void VPlot::messageLog(QString logmess, bool timest)
{
    if (timest)
        mngViewBox->ui->mMsgsLog->insertPlainText(QDateTime::currentDateTime().toString("hh:mm:ss")+" "+logmess+"\n");
    else
        mngViewBox->ui->mMsgsLog->insertPlainText(logmess+"\n" );

    mngViewBox->ui->mMsgsLog->verticalScrollBar()->setValue(mngViewBox->ui->mMsgsLog->verticalScrollBar()->maximum());
}

int VPlot::counts(void)
{
    return mGraph0->dataCount();
}

bool VPlot::isDataHave(void)
{
    if ( mGraph0->dataCount() > 0 )
        return true;
    else
        return false;
}

void VPlot::setRangeX(bool replot)
{
    m_rangeX0 = mw->ui->lineEditAxX0->text().toInt();
    m_rangeXS = mw->ui->lineEditAxXsize->text().toInt();

    axisXB0->setRange(m_rangeX0,  m_rangeXS, Qt::AlignLeft);

    if(replot)
        mPlot->replot();
}

void VPlot::setRangeY(bool replot)
{
    m_minVG0 = mw->ui->lineEditAxYmin->text().toDouble();
    m_maxVG0 = mw->ui->lineEditAxYmax->text().toDouble();

    axisYL0->setRange(m_minVG0, m_maxVG0);
    axisYR0->setRange(m_minVG0, m_maxVG0);

    if(replot)
        mPlot->replot();
}

void VPlot::setRange(void)
{
    setRangeX();
    setRangeY();
    mPlot->replot();
}

void VPlot::showCoord(void)
{
    QCPRange qcprange = mPlot->xAxis->range();

    mw->ui->lineEditAxX0->setText(QString().setNum((int)qcprange.lower));
    mw->ui->lineEditAxXsize->setText(QString().setNum((int)m_rangeXS));

    qcprange = axisYL0->range();
    mw->ui->lineEditAxYmin->setText(QString().setNum(qcprange.lower,'e',2));
    mw->ui->lineEditAxYmax->setText(QString().setNum(qcprange.upper,'e',2));
}

void VPlot::setScaleF(void)
{
    if(mw->opt->uiopt->checkBAutoScaleY->isChecked())
    {
        showCoord();
        setRange();
    }

    if(mw->opt->uiopt->checkBAutoSaveSet->isChecked())
        mw->opt->saveSettings();
}

void VPlot::cleanPlot(bool fullclean)
{
    if (isDataHave())
    {
        for( int igraph=0; igraph<mPlot->graphCount(); ++igraph )
        {
            mPlot->graph(igraph)->data().data()->clear();
        }
    }

    if (fullclean)
    {
        closeFileSave();
        createFileSave(fullclean);
        mngViewBox->ui->mDataLog->clear();
        tmX0 = (double) (QDateTime::currentDateTime().toMSecsSinceEpoch());
    }
    else
    {
        messageLog("Clean Plot");
        mngViewBox->ui->mDataLog->clear();
    }

    mPlot->replot();
}

void VPlot::savePlotGraph(const QString &fileName, const QString &ext)
{
    QString filetosave = mw->curset.pathdata+'/'+fileName+"."+ext;

    QFile file(filetosave);

    if (file.open(QIODevice::WriteOnly))
    {
        if (ext == "png")
            mPlot->savePng(filetosave);
        if (ext == "jpg")
            mPlot->saveJpg(filetosave);
        if (ext == "bmp")
            mPlot->saveBmp(filetosave);
        if (ext == "pdf")
            mPlot->savePdf(filetosave);

        messageLog(fileName+"."+ext);

        file.flush();
        file.close();
    }
    else
        qDebug() << file.errorString();
}

void VPlot::on_pushBSavePlot_release(void)
{
    QString fileName(mw->curset.namedatafile.split('.')[0]);

    messageLog("Save plot: " + mw->curset.pathdata);

    if (mngViewBox->ui->checkBoxPNG->isChecked())
        savePlotGraph(fileName,QString("png"));
    if (mngViewBox->ui->checkBoxJPG->isChecked())
        savePlotGraph(fileName,QString("jpg"));
    if (mngViewBox->ui->checkBoxBMP->isChecked())
        savePlotGraph(fileName,QString("bmp"));
    if (mngViewBox->ui->checkBoxPDF->isChecked())
        savePlotGraph(fileName,QString("pdf"));
}

void VPlot::point(QCPGraph* dwPoints, int x, int y, bool setPt, const QColor &color)
{
    double xd = axisXB0->pixelToCoord(x);
    double yd = axisYL0->pixelToCoord(y);

    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::black);
    pal.setColor(QPalette::Window, color);
    msWl->setAutoFillBackground(true);
    msWl->setText(QString().setNum(yd,'e',2));
    msWl->setPalette(pal);
    msWl->setAlignment(Qt::AlignCenter);
    msWl->setGeometry(x+15,y+20,60,20);

    if (setPt)
    {
        QDateTime tmx;
        tmx.setTimeZone(QTimeZone::utc());
        tmx.setMSecsSinceEpoch((qint64)xd);

        messageLog("Set point: " + tmx.toString("hh:mm:ss") + " " + QString().setNum(yd,'e',2));

        //Plot points at these locations:
        if(dwPoints == nullptr)
            dwPoints = new QCPGraph(mPlot->xAxis, axisYL0);
        dwPoints->setAdaptiveSampling(false);
        dwPoints->setLineStyle(QCPGraph::lsNone);
        dwPoints->setScatterStyle(QCPScatterStyle::ssCircle);
        dwPoints->setPen(QPen(QBrush(Qt::red), 2));
        dwPoints->addData(xd, yd);
    }
}

void VPlot::setPoint(QMouseEvent *event, QCPGraph* dwPoints, bool setPt, const QColor &color)
{
    point(dwPoints,event->pos().x(), event->pos().y(), setPt, color);
}

void VPlot::setPoint(QWheelEvent *event, QCPGraph* dwPoints, bool setPt, const QColor &color)
{
    point(dwPoints,event->position().x(), event->position().y(), setPt, color);
}

void VPlot::on_MouseMove(QMouseEvent *event)
{
    if(mWPoints != nullptr)
        mWPoints->data()->clear();

    setPoint(event, mMPoints, false, QColor(0xA0,0xFF,0xA0));

    if(mw->opt->uiopt->checkBAutoScaleY->isChecked())
        showCoord();

    mPlot->replot();
    Move = true;
}

void VPlot::on_MousePress(QMouseEvent *event)
{
    event->accept();
    Press = true;
}

void VPlot::on_MouseRelease(QMouseEvent *event)
{
    if(mw->mRun)
        if (!(Move && Press))
            setPoint(event,mRPoints, true, QColor(0xA0,0xFF,0xA0));

    if (Move && Press)
        mw->ui->pushBScaleY->setEnabled(true);

    setScaleF();
    mPlot->replot();
    Move = Press = false;
 }

void VPlot::on_MouseWheel(QWheelEvent *event)
{
    if(mw->mRun)
        setPoint(event, mWPoints, false, QColor(0xFF,0xA0,0xA0));

    QCPRange qcprange = mPlot->xAxis->range();
    m_rangeXS = qcprange.upper - qcprange.lower;
    setScaleF();

    mPlot->replot();
    mw->ui->pushBScaleY->setEnabled(true);
}

//Printing preview and printing
void VPlot::doPrintPreview()
{
    QPrinter printer;
    QPrintPreviewDialog preview (&printer, this);
    connect(&preview,&QPrintPreviewDialog::paintRequested,this,&VPlot::printPreview);
    preview.exec();
}

void VPlot::printPreview(QPrinter *printer)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    printer->setPageSize(QPageSize::A4);
#else
    printer->setPaperSize(QPrinter::A4);
#endif
    printer->setPageOrientation(QPageLayout::Landscape);
    printer->setPageMargins(QMarginsF(0.75, 0.75, 0.75, 0.75), QPageLayout::Unit::Inch);
    //printer->setOutputFormat(QPrinter::PdfFormat);

    QCPPainter painter(printer);
    QRectF pageRect = printer->pageRect(QPrinter::DevicePixel);

    // set size on page
    int plotWidth = mPlot->viewport().width();
    int plotHeight = mPlot->viewport().height();

    double scale = pageRect.width() / (double)plotWidth;
    double scale2 = pageRect.height() / (double)plotHeight;

    if (scale2 < scale)
        scale = scale2;

    painter.setMode(QCPPainter::pmVectorized);
    painter.setMode(QCPPainter::pmNoCaching);
    painter.setMode(QCPPainter::pmNonCosmetic);

    painter.scale(scale, scale);
    mPlot->toPainter(&painter, plotWidth, plotHeight);
    painter.drawText(-10, -10, QDateTime::currentDateTime().toString());
}

void VPlot::setOpenGL(bool state)
{
    mPlot->setOpenGl(state);
    mPlot->openGl() ? mw->SBarOGLst->setText("ON") : mw->SBarOGLst->setText("OFF");
    mPlot->replot();
}
