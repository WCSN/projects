#include <QLayout>
#include <QLabel>
#include <QFrame>
#include <QSplitter>
#include <QTextEdit>
#include <QDateTime>
#include <QTextStream>

#include "addons.h"
#include "vplot.h"

VPlot::VPlot(QWidget *parent, Ui_MainWindow *UI) :
    QWidget( parent ),
    m_minVG0(4.00e-4),
    m_maxVG0(9.00e-4),
    ui(UI)
{
    ui->layoutGridPlot->setMargin(3);
    ui->layoutGridPlot->setSpacing(3);

    ui->lineEditAxYMin->setText(QString::number(m_minVG0,'e',2));
    ui->lineEditAxYMax->setText(QString::number(m_maxVG0,'e',2));

    static const char* const FILE_NAME = "brga.csv";

    ui->lineEditFileDataSave->setText(FILE_NAME);

    dataoutfile = new QFile( FILE_NAME );

    if(dataoutfile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        dataoutstream = new QTextStream (dataoutfile);
        dataoutstream->setCodec("UTF-8");
        *dataoutstream <<  tr("#N;Time;Data") << Qt::endl;
        dataoutstream->flush();
    }

    splPlot = new QSplitter( Qt::Horizontal );
    mPlot = new QCustomPlot(parent);
    CreatePlotter();

    ptxt = new QTextBrowser;
    plog = new QTextBrowser;

    countpoint = new WLabel( NULL, mPlot, Qt::green, Qt::black );
    countpoint->setGeometry( 50, 20, 100, 20 );

    messages = new WLabel("Disconnect", mPlot, Qt::red, Qt::black, 300);
    messages->setGeometry( 50, 40, 100, 20 );

    spltxtview = new QSplitter( Qt::Vertical );
    spltxtview->addWidget( ptxt );
    spltxtview->addWidget( plog );
    spltxtview->setStretchFactor( 0, 1000 );

    splPlot->addWidget( mPlot );
    splPlot->addWidget( spltxtview );
    splPlot->setStretchFactor( 0, 1000 );

    ui->layoutGridPlot->addWidget( splPlot );

    textItem = new QCPItemText(mPlot);

    connect( mPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),mPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(mPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)), mPlot->yAxis, SLOT(setRange(QCPRange)));
}

VPlot::~VPlot()
{
    dataoutfile->close();
}

void VPlot::Texttolog(const QString outtext)
{
    *dataoutstream << outtext << Qt::endl;
    dataoutstream->flush();
}

void VPlot::CreatePlotter( void )
{
//    mPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
//    mPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика

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
    mPlot->xAxis->setTicker(aXtmTick);
    mPlot->xAxis->setSubTicks(true);

    mPlot->yAxis->setVisible( true );
    mPlot->yAxis->setTickLabels( true );
    mPlot->yAxis->setSubTicks(true);
    mPlot->yAxis->setTickLabelRotation(0);
    mPlot->yAxis->ticker()->setTickCount(9);
    mPlot->yAxis->setNumberFormat("eb");
    mPlot->yAxis->setNumberPrecision(2);
    mPlot->yAxis->setRange(m_minVG0, m_maxVG0);

    mPlot->yAxis2->setVisible( true );
    mPlot->yAxis2->setTickLabels( false );
    mPlot->yAxis2->setTickLabelRotation(0);
    mPlot->yAxis2->ticker()->setTickCount(9);
    //mPlot->yAxis2->setNumberFormat("eb");
    //mPlot->yAxis2->setNumberPrecision(2);
    mPlot->yAxis2->setRange(m_minVG0, m_maxVG0);

    // configure plot to have 4 right axes:
//    mPlot->axisRect()->addAxis( QCPAxis::atRight );
//    mPlot->axisRect()->addAxis( QCPAxis::atRight );
//    mPlot->axisRect()->addAxis( QCPAxis::atRight );
//    mPlot->axisRect()->addAxis( QCPAxis::atRight );

    mPlot->axisRect()->axis( QCPAxis::atRight, 0)->setPadding( 80 ); // add some padding to have space for tags
//    mPlot->axisRect()->axis( QCPAxis::atRight, 0)->setRange(4.0e-4, 10.0e-4);  ?????????
//
//    mPlot->axisRect()->axis( QCPAxis::atRight, 1)->setPadding( 20 ); // add some padding to have space for tags
//    mPlot->axisRect()->axis( QCPAxis::atRight, 1)->setRange(0, 10);

//    mPlot->axisRect()->axis( QCPAxis::atRight, 2)->setPadding( 20 ); // add some padding to have space for tags
//    mPlot->axisRect()->axis( QCPAxis::atRight, 2)->setRange(0, 10);

//    mPlot->axisRect()->axis( QCPAxis::atRight, 3)->setPadding( 20 ); // add some padding to have space for tags
//    mPlot->axisRect()->axis( QCPAxis::atRight, 3)->setRange(0, 10);

    // create graphs:
    mGraph0 = mPlot->addGraph( mPlot->xAxis, mPlot->axisRect()->axis( QCPAxis::atLeft, 0 ) );
//    mGraph1 = mPlot->addGraph( mPlot->xAxis, mPlot->axisRect()->axis( QCPAxis::atLeft, 1 ) );
//    mGraph2 = mPlot->addGraph( mPlot->xAxis, mPlot->axisRect()->axis( QCPAxis::atLeft, 2 ) );
//    mGraph3 = mPlot->addGraph( mPlot->xAxis, mPlot->axisRect()->axis( QCPAxis::atLeft, 3 ) );

    double tmE = (double) QDateTime::currentSecsSinceEpoch();

    // mPlot->xAxis->setRange( mPlot->xAxis->range().upper, tmE-m_rangeX, Qt::AlignRight );
    mPlot->xAxis->setRange( tmE, m_rangeX, Qt::AlignLeft );

    QPen pen; pen.setWidth(1);

    pen.setColor(QColor( 0xAA, 0x00, 0x00 ));
    mGraph0->setPen( pen );
//    pen.setColor(QColor( 0x00, 0xAA, 0x00 ));
//    mGraph1->setPen( pen );
//    pen.setColor(QColor( 0x00, 0x00, 0xAA ));
//    mGraph2->setPen( pen );
//    pen.setColor(QColor( 0x00, 0x00, 0x00 ));
//    mGraph3->setPen( pen );

    // create tags with newly introduced AxisTag class (see axistag.h/.cpp):
    mTag0 = new AxisTag(mGraph0->keyAxis());
    mTag0->setPen( mGraph0->pen() );
    mTag0->setText(QString().setNum(m_minVG0,'e',2));
    mTag0->updatePosition(m_minVG0);

//    mTag1 = new AxisTag( mGraph1->valueAxis());
//    mTag1->setPen( mGraph1->pen() );
//    mTag1->setText("0.000");

//    mTag2 = new AxisTag( mGraph2->valueAxis());
//    mTag2->setPen( mGraph2->pen() );
//    mTag2->setText("0.000");

//    mTag3 = new AxisTag( mGraph3->valueAxis());
//    mTag3->setPen( mGraph3->pen() );
//    mTag3->setText("0.000");
}

void VPlot::mPlotUpdate(void)
{
    if( mvalY.empty() || mvalY.size() != 4 ) return;

    double V0 = mvalY[1].toDouble();//*10000.0; // !!! change for all or other data received
//    double V1 = mvalY[1].toDouble()*10000.0;
//    double V2 = mvalY[2].toDouble()*10000.0;
//    double V3 = mvalY[3].toDouble()*10000.0;

    double tmE = (double) QDateTime::currentSecsSinceEpoch();
    QTime tm = QTime::currentTime();
    mGraph0->addData( tmE, V0 );
//    mGraph1->addData( mGraph1->dataCount(), V1 );
//    mGraph2->addData( mGraph2->dataCount(), V2 );
//    mGraph3->addData( mGraph3->dataCount(), V3 );

    mGraph0->rescaleAxes(true);
//    mGraph1->rescaleAxes(true);
//    mGraph2->rescaleAxes(true);
//    mGraph3->rescaleAxes(true);

    // Scale for all data min/max
//    mGraph0->rescaleValueAxis( false, true );
//    mGraph1->rescaleValueAxis( false, true );
//    mGraph2->rescaleValueAxis( false, true );
//    mGraph3->rescaleValueAxis( false, true );

//    mPlot->xAxis->setRange( mPlot->xAxis->range().upper, tmE-m_rangeX, Qt::AlignRight );
//    mPlot->xAxis->setRange( tmE, m_rangeX, Qt::AlignRight );
    mPlot->xAxis->setRange( tmE, m_rangeX, Qt::AlignLeft );
//    mPlot->xAxis->setRange( mPlot->xAxis->range().upper, m_rangeX, Qt::AlignRight );
    mPlot->xAxis->rescale();

    // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
    double graph0Value = mGraph0->dataMainValue( mGraph0->dataCount() - 1 );
//    double graph1Value = mGraph1->dataMainValue( mGraph1->dataCount() - 1 );
//    double graph2Value = mGraph2->dataMainValue( mGraph2->dataCount() - 1 );
//    double graph3Value = mGraph3->dataMainValue( mGraph3->dataCount() - 1 );

    mTag0->updatePosition( graph0Value );
//    mTag1->updatePosition( graph1Value );
//    mTag2->updatePosition( graph2Value );
//    mTag3->updatePosition( graph3Value );

    mTag0->setText( QString().setNum(graph0Value, 'e', 2) );
//    mTag1->setText( QString().setNum(graph1Value, 'e', 2) );
//    mTag2->setText( QString().setNum(graph2Value, 'e', 2) );
//    mTag3->setText( QString().setNum(graph3Value, 'e', 2) );

    mPlot->replot();
    mvalY.clear();

    countpoint->setNum( mGraph0->dataCount() );

    ptxt->insertPlainText( QString(countpoint->text() + ". " + tm.toString("hh:mm:ss") + "   " + mTag0->text() + "\n") );
    ptxt->verticalScrollBar()->maximum();

    Texttolog(QString(countpoint->text() + ";" + tm.toString("hh:mm:ss") + ";" + mTag0->text()));
}

void VPlot::MessageBox(const QString message,const QColor &tcolor, const QColor &bcolor)
{
    messages->wsetColors(tcolor, bcolor);
    messages->wsetText(message);
}

void VPlot::MessageLog(QString logmess)
{
    plog->insertPlainText( logmess+"\n" );
    plog->verticalScrollBar()->maximum();
}

void VPlot::SetData(const QString *data)
{
    mvalY = data->split(",");
    mPlotUpdate();
}

void VPlot::SetRange(void)
{
    m_minVG0=ui->lineEditAxYMin->text().toDouble();
    m_maxVG0=ui->lineEditAxYMax->text().toDouble();
    mPlot->yAxis2->setRange(m_minVG0, m_maxVG0);
    mPlot->replot();
}
