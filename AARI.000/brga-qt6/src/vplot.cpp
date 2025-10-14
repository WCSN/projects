#include <QLayout>
#include <QLabel>
#include <QFrame>
#include <QSplitter>
#include <QTextEdit>
#include <QDateTime>
#include <QTextStream>
#include <QWheelEvent>
#include <QPalette>

#include "vplot.h"
#include "settings.h"
//
//  Не забудь, что почистил всю многографиксность.
//
VPlot::VPlot(QWidget *parent, MainWindow *MW, Ui_MainWindow *UI) :
    QWidget( parent ),
    ui(UI),
    mw(MW)
{
    ui->layoutGridPlot->setContentsMargins(3,3,3,3);
    ui->layoutGridPlot->setSpacing(3);

    ui->lineEditAxYMin->setText(QString::number(m_minVG0,'e',2));
    ui->lineEditAxYMax->setText(QString::number(m_maxVG0,'e',2));

    splPlot = new QSplitter( Qt::Horizontal );

    mPlot = new QCustomPlot(parent);

    CreatePlotter();

    ptxt = new QTextBrowser;
    plog = new QTextBrowser;

    countpoint = new WLabel( NULL, this, Qt::green, Qt::black );
    countpoint->setGeometry( 0, 0, 45, 20 );

    messages = new WLabel("Disconnect", this, Qt::red, Qt::black, 450);
    messages->setGeometry( 0, 0, 120, 20 );

    splInd = new QSplitter( Qt::Horizontal );

    splInd->addWidget( countpoint );
    splInd->addWidget( messages );

    spltxtview = new QSplitter( Qt::Vertical );
    spltxtview->addWidget( splInd );
    spltxtview->addWidget( ptxt );
    spltxtview->addWidget( plog );

    spltxtview->setStretchFactor( 0, 4 );
    spltxtview->setStretchFactor( 1, 10 );
    spltxtview->setStretchFactor( 2, 6 );

    splPlot->addWidget( mPlot );
    splPlot->addWidget( spltxtview );

    splPlot->setStretchFactor( 0, 100 );

    ui->layoutGridPlot->addWidget( splPlot );

    //textItem = new QCPItemText(mPlot);
    msMv = new QLabel(mPlot);
    msWl = new QLabel(mPlot);

    connect( mPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),mPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(mPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)), mPlot->yAxis, SLOT(setRange(QCPRange)));
    connect(mPlot, &QCustomPlot::mouseMove, this, &VPlot::on_MouseMove);
    connect(mPlot, &QCustomPlot::mouseWheel, this,&VPlot::on_MouseWheel);
    connect(mPlot, &QCustomPlot::mousePress, this,&VPlot::on_MousePress);
    connect(mPlot, &QCustomPlot::mouseRelease, this,&VPlot::on_MouseRelease);
}

VPlot::~VPlot()
{
    CloseFileSave();
}

void VPlot::initSettings(void)
{
    m_minVG0 = Settings::value("Plotter/AxisYMin").toDouble();
    m_maxVG0 = Settings::value("Plotter/AxisYMax").toDouble();

    filedatasave.clear();
    filedatasave.push_back(ui->lineEditFileDataSave->text());
}

void VPlot::CreateFileSave(void)
{
    if (dataoutfile == nullptr)
    {
        dataoutfile = new QFile(filedatasave);

        if(dataoutfile->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            dataoutstream = new QTextStream (dataoutfile);
            //dataoutstream->setCodec("UTF-8");
            *dataoutstream <<  tr("#N;Time;TimeExp;Data;") << Qt::endl;
            dataoutstream->flush();
        }
    }
}

void VPlot::CloseFileSave(void)
{
    if (dataoutfile != nullptr)
        dataoutfile->close();
}

void VPlot::CreatePlotter( void )
{
//    mPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
//    mPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    initSettings();

    mPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                           QCP::iSelectLegend | QCP::iSelectPlottables);

    mPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);   // Включаем перетаскивание
    mPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);   // Включаем удаление/приближение

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(230, 245, 230));
    plotGradient.setColorAt(1, QColor(230, 245, 230));
    mPlot-> setBackground (plotGradient); // Устанавливаем цвет фона

    QSharedPointer<QCPAxisTickerDateTime> aXtmTick(new QCPAxisTickerDateTime);
    aXtmTick->setDateTimeFormat("hh:mm:ss");
    aXtmTick->setTimeZone(QTimeZone::utc());
    mPlot->xAxis->setTicker(aXtmTick);
    mPlot->xAxis->setSubTicks(true);
    mPlot->xAxis->setRange( 0.0, m_rangeX);

    mPlot->yAxis->setVisible( true );
    mPlot->yAxis->setTickLabels( true );
    mPlot->yAxis->setSubTicks(true);
    mPlot->yAxis->setTickLabelRotation(0);
    mPlot->yAxis->ticker()->setTickCount(9);
    mPlot->yAxis->setNumberFormat("eb");
    mPlot->yAxis->setNumberPrecision(2);
    mPlot->yAxis->setRange(m_minVG0, m_maxVG0);

    mPlot->yAxis2->setVisible( true );
    mPlot->yAxis2->setTickLabels( true );
    mPlot->yAxis2->setSubTicks(true);
    mPlot->yAxis2->setTickLabelRotation(0);
    mPlot->yAxis2->ticker()->setTickCount(9);
    mPlot->yAxis2->setNumberFormat("eb");
    mPlot->yAxis2->setNumberPrecision(2);
    mPlot->yAxis2->setRange(m_minVG0, m_maxVG0);

    // Colors
    mPlot->xAxis->setBasePen(QPen(Qt::black, 1));  // Горизонтальные и вертикальные линии оси координат
    mPlot->yAxis->setBasePen(QPen(Qt::black, 1));  // Горизонтальные и вертикальные линии оси координат
    mPlot->xAxis->setTickPen(QPen(Qt::green, 1));  // Цвет маленькой галочки над числом на оси
    mPlot->yAxis->setTickPen(QPen(Qt::green, 1));  // Цвет маленькой галочки над числом на оси
    mPlot->xAxis->setSubTickPen(QPen(Qt::red, 1)); // Цвет мелкой шкалы и конечной стрелки на оси
    mPlot->yAxis->setSubTickPen(QPen(Qt::red, 1)); // Цвет мелкой шкалы и конечной стрелки на оси

    mPlot->xAxis->setTickLabelColor(Qt::blue);  // Числа на осях
    mPlot->yAxis->setTickLabelColor(Qt::blue);
    mPlot->yAxis2->setTickLabelColor(Qt::blue);

    mPlot->xAxis->grid()->setPen(QPen(QColor(140, 0, 0), 1, Qt::DotLine)); // Формат рисования пунктирной линии по оси X номер (крупный масштаб)
    mPlot->yAxis->grid()->setPen(QPen(QColor(140, 0, 0), 1, Qt::DotLine)); // Формат рисования пунктирной линии, проходящей справа от номера оси Y (крупный масштаб)
    mPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine)); // Расширение формата рисования линии там, где нет числа на оси X (мелкий масштаб)
    mPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine)); // Формат рисования линии по оси Y справа, где нет числа (мелкий масштаб)
    mPlot->xAxis->grid()->setSubGridVisible(true); // Есть ли на графике пунктирная линия сетки
    mPlot->yAxis->grid()->setSubGridVisible(true); // Есть ли на графике пунктирная линия сетки
    mPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen); // Устанавливаем перо нулевой линии, то есть вертикальную линию на 0 по оси X
    mPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen); // Устанавливаем перо нулевой линии, которое представляет собой горизонтальную линию в 0 на оси Y
    // ===========

    mPlot->axisRect()->axis( QCPAxis::atRight, 0)->setPadding( 13 ); // add some padding to have space for tags
    mGraph0 = mPlot->addGraph( mPlot->xAxis, mPlot->axisRect()->axis( QCPAxis::atLeft, 0 ) );

    QPen pen; pen.setWidth(1);
    pen.setColor(QColor( 0xAA, 0x00, 0x00 ));
    mGraph0->setPen( pen );

    mTag0 = new AxisTag(mGraph0->valueAxis());
    // set tags with newly introduced AxisTag class (see axistag.h/.cpp):
    mTag0->setPen(mGraph0->pen());
    mTag0->setText(QString().setNum(m_minVG0,'e',2));
//    mTag0->updatePosition(m_minVG0);

    QDateTime tmx; tmx.setTimeZone(QTimeZone::utc());
    tmEX0 = (double)tmx.currentSecsSinceEpoch();
    tmEXN=tmEX0; // On init
}

void VPlot::mPlotUpdate(void)
{
    if( mvalY.empty() || mvalY.size() != 4 ) return;

    double V0 = mvalY[1].toDouble();//*10000.0; // !!! change for all or other data received

    QDateTime tm = QDateTime::currentDateTime();
    QDateTime tmx; tmx.setTimeZone(QTimeZone::utc());

    double tmEX1 = tmEXN - tmEX0;
    mGraph0->rescaleAxes(true);
    mGraph0->addData( tmEX1, V0 );
    mPlot->xAxis->setRange( 0.0, tmEX1); //, Qt::AlignLeft );
    mPlot->xAxis->rescale();

    // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
    double graph0Value = mGraph0->dataMainValue( mGraph0->dataCount() - 1 );

    mTag0->updatePosition( graph0Value );
    mTag0->setText( QString().setNum(graph0Value, 'e', 2) );
    mPlot->replot();

    mvalY.clear();

    tmx.setSecsSinceEpoch(tmEX1);

    ptxt->insertPlainText( QString().setNum(mGraph0->dataCount()) + ". " + tm.toString("hh:mm:ss") + "  " + tmx.toString("hh:mm:ss") + " " + mTag0->text() + "\n");
    ptxt->verticalScrollBar()->setValue(ptxt->verticalScrollBar()->maximum());
    Datatocvs(QString().setNum(mGraph0->dataCount()) + ";" + tm.toString("hh:mm:ss") + ";" + tmx.toString("hh:mm:ss") + ";" + mTag0->text() + ";");

    tmEXN = (double)tmx.currentSecsSinceEpoch();
}

void VPlot::Datatocvs(const QString &outtext)
{
    *dataoutstream << outtext << Qt::endl;
    dataoutstream->flush();
}

void VPlot::MessageLog(QString logmess)
{
    plog->insertPlainText( QDateTime::currentDateTime().toString("hh:mm:ss")+" "+logmess+"\n" );
    plog->verticalScrollBar()->setValue(plog->verticalScrollBar()->maximum());
}

void VPlot::PlotDataUpdate(const QString *data)
{
    mvalY = data->split(",");
    mPlotUpdate();
}

int VPlot::counts(void)
{
    return mGraph0->dataCount();
}

void VPlot::SetRange(void)
{
    m_minVG0=ui->lineEditAxYMin->text().toDouble();
    m_maxVG0=ui->lineEditAxYMax->text().toDouble();
    mPlot->yAxis->setRange(m_minVG0, m_maxVG0);
    mPlot->yAxis2->setRange(m_minVG0, m_maxVG0);
}

void VPlot::ScaleFset(void)
{
    QCPRange qcprange = mPlot->yAxis->range();
    ui->lineEditAxYMin->setText(QString().setNum(qcprange.lower,'e',2));
    ui->lineEditAxYMax->setText(QString().setNum(qcprange.upper,'e',2));
    SetRange();
}

void VPlot::CleanPlot(void)
{
    for( int igraph=0; igraph<mPlot->graphCount(); ++igraph )
    {
        mPlot->graph(igraph)->data().data()->clear();
    }
    if (ui->checkBAutoNameFile->isChecked())
    {
        ui->lineEditFileDataSave->setText("brga-"+CurrentTime("yyyyMMdd-hhmmss")+".csv");
        filedatasave = ui->lineEditFileDataSave->text();
    }

    MessageLog("Data save: "+filedatasave);
    delete mTag0;
    CloseFileSave();
    CreatePlotter();
    CreateFileSave();
    ptxt->clear();
    mPlot->replot();
}

void VPlot::on_MouseMove(QMouseEvent *event)
{
    //if(!mw->mRun) return;

    //double xd = mPlot->xAxis->pixelToCoord(event->pos().x());
    double yd = mPlot->yAxis->pixelToCoord(event->pos().y());

    int x = event->pos().x();
    int y = event->pos().y();

    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::blue);
    pal.setColor(QPalette::Window, QColor(0xF0,0xFF,0xA0)); // #f8ffe9
    msMv->setAutoFillBackground(true);
    msMv->setText(QString().setNum(yd,'e',2));
    msMv->setPalette(pal);
    msMv->setAlignment(Qt::AlignCenter);
    msMv->setGeometry(x+15,y+10,60,20);

    ScaleFset();
    if(ui->checkBAutoScale->isChecked())
        mw->SaveSettings();

    mPlot->replot();
}

void VPlot::on_MouseWheel(QWheelEvent *event)
{
    if(mw->mRun)
    {
        //double xd = mPlot->xAxis->pixelToCoord(event->position().x());
        double yd = mPlot->yAxis->pixelToCoord(event->position().y());

        int x = event->position().x();
        int y = event->position().y();

        QPalette pal;
        pal.setColor(QPalette::WindowText, Qt::blue);
        pal.setColor(QPalette::Window, QColor(0xFF,0xE0,0xE0)); // #f8ffe9
        msWl->setAutoFillBackground(true);
        msWl->setText(QString().setNum(yd,'e',2));
        msWl->setPalette(pal);
        msWl->setAlignment(Qt::AlignCenter);
        msWl->setGeometry(x+15,y+30,60,20);
    }

    mPlot->replot();

    ScaleFset();
    if(ui->checkBAutoScale->isChecked())
        mw->SaveSettings();
}

void VPlot::on_MousePress(QMouseEvent *event)
{
    event->ignore();
}

void VPlot::on_MouseRelease(QMouseEvent *event)
{
    if(!mw->mRun) return;

    double xd = mPlot->xAxis->pixelToCoord(event->pos().x());
    double yd = mPlot->yAxis->pixelToCoord(event->pos().y());

    int x = event->pos().x();
    int y = event->pos().y();

    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::black);
    pal.setColor(QPalette::Base, QColor(0xE0,0xE0,0xFF)); // #f8ffe9
    msWl->setAutoFillBackground(true);
    msWl->setText(QString().setNum(yd,'e',2));
    msWl->setPalette(pal);
    msWl->setAlignment(Qt::AlignCenter);
    msWl->setGeometry(x+15,y,60,20);

    QVector<double> xDt;
    QVector<double> yDt;

    xDt.push_back(xd);
    yDt.push_back(yd);

    QDateTime tmx;
    tmx.setTimeZone(QTimeZone::utc());
    tmx.setSecsSinceEpoch((qint64)xd);

    MessageLog("Set point: " + tmx.toString("hh:mm:ss") + " " + QString().setNum(yd,'e',2));

    //Plot points at these locations:
    QCPGraph* dwPoints = new QCPGraph(mPlot->xAxis, mPlot->yAxis);
    dwPoints->setAdaptiveSampling(false);
    dwPoints->setLineStyle(QCPGraph::lsNone);
    dwPoints->setScatterStyle(QCPScatterStyle::ssCircle);
    dwPoints->setPen(QPen(QBrush(Qt::red), 2));
    dwPoints->addData(xDt, yDt);

    mPlot->replot();
}
