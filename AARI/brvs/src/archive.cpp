#include "archive.h"

#include <qlayout.h>
#include <qlabel.h>
#include <QFrame>
#include <QSplitter>
#include <QTextEdit>
#include <QTimer>
#include "addons.h"


Archive::Archive( QWidget *parent, Ui_MainWindow *ui ) :
		QWidget( parent ), uip( ui )
{
	QGridLayout *layoutGlob = new QGridLayout( parent );

	QSplitter *spl = new QSplitter( Qt::Horizontal );

	QWidget *area01 = new QWidget();
	QGridLayout *layout = new QGridLayout(area01);
	layout->setMargin(0);
	layout->setSpacing(0);

	mPlot = new QCustomPlot(area01);
	CreatePlotter();
	connect( &mDataTimer, SIGNAL( timeout() ), this, SLOT( mPlotSlot() ) );

	QTextEdit* ptxt = new QTextEdit;
	spl->addWidget( ptxt );

	ptxt->setPlainText( "file_ddmmyy.brd\n"
		"file_ddmmyy.brd\n"
		"file_ddmmyy.brd\n"
		"file_ddmmyy.brd\n"
		"file_ddmmyy.brd\n"
		"file_ddmmyy.brd\n"
		"file_ddmmyy.brd\n" );

	countpoint = new WLabel( NULL, mPlot, Qt::green, Qt::black );
	countpoint->setGeometry( 50, 20, 140, 20 );
	//lb->setFrameStyle(1);

	messages = new WLabel(QString::fromUtf8("Это имитационные данные"), mPlot, Qt::red, Qt::lightGray, 300);
	messages->setGeometry( 50, 40, 140, 20 );

	layout->addWidget(uip->btnRunA,0,0);
	layout->addWidget(uip->kledRunA,0,1);
	layout->addWidget(mPlot,1,0,1,3);

	spl->addWidget( area01 );
	spl->addWidget( ptxt );
	spl->setStretchFactor( 0, 1000 );

	layoutGlob->addWidget( spl );
}

void Archive::CreatePlotter( void )
{
	// configure plot to have 3 right axes:
	mPlot->yAxis->setTickLabels( false );
	connect( mPlot->yAxis2, SIGNAL( rangeChanged(QCPRange) ), mPlot->yAxis, SLOT( setRange(QCPRange) ) ); // left axis only mirrors inner right axis

	mPlot->yAxis2->setVisible( true );

	mPlot->axisRect()->addAxis( QCPAxis::atRight );
	mPlot->axisRect()->addAxis( QCPAxis::atRight );

	mPlot->axisRect()->axis( QCPAxis::atRight, 0 )->setPadding( 20 ); // add some padding to have space for tags
	mPlot->axisRect()->axis( QCPAxis::atRight, 1 )->setPadding( 20 ); // add some padding to have space for tags
	mPlot->axisRect()->axis( QCPAxis::atRight, 2 )->setPadding( 20 ); // add some padding to have space for tags

	// create graphs:
	mGraph1 = mPlot->addGraph( mPlot->xAxis, mPlot->axisRect()->axis( QCPAxis::atRight, 0 ) );
	mGraph2 = mPlot->addGraph( mPlot->xAxis, mPlot->axisRect()->axis( QCPAxis::atRight, 1 ) );
	mGraph3 = mPlot->addGraph( mPlot->xAxis, mPlot->axisRect()->axis( QCPAxis::atRight, 2 ) );

	mGraph1->setPen( QPen( QColor( 0xAA, 0x00, 0x00 ) ) );
	mGraph2->setPen( QPen( QColor( 0x00, 0xAA, 0x00 ) ) );
	mGraph3->setPen( QPen( QColor( 0x00, 0x00, 0xAA ) ) );

	// create tags with newly introduced AxisTag class (see axistag.h/.cpp):
	mTag1 = new AxisTag( mGraph1->valueAxis() );
	mTag1->setPen( mGraph1->pen() );

	mTag2 = new AxisTag( mGraph2->valueAxis() );
	mTag2->setPen( mGraph2->pen() );

	mTag3 = new AxisTag( mGraph3->valueAxis() );
	mTag3->setPen( mGraph3->pen() );
}

void Archive::mPlotSlot()
{
// calculate and add a new data point to each graph:
	mGraph1->addData( mGraph1->dataCount(), qFastSin( mGraph1->dataCount() / 50.0 ) );

	countpoint->setNum( mGraph1->dataCount() );

	mGraph2->addData(
	    mGraph2->dataCount(), qCos( mGraph2->dataCount() / 50.0 ) + qSin( mGraph2->dataCount() / 50.0 / 0.4364 ) * 0.15 );

	mGraph3->addData(
	    mGraph3->dataCount(), qCos( mGraph3->dataCount() / 50.0 ) + qCos( mGraph3->dataCount() / 50.0 / 0.3364 ) * 0.20 );

// make key axis range scroll with the data:
	mPlot->xAxis->rescale();
	mGraph1->rescaleValueAxis( false, true );
	mGraph2->rescaleValueAxis( false, true );
	mGraph3->rescaleValueAxis( false, true );

	mPlot->xAxis->setRange( mPlot->xAxis->range().upper, 100, Qt::AlignRight );

// update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
	double graph1Value = mGraph1->dataMainValue( mGraph1->dataCount() - 1 );
	double graph2Value = mGraph2->dataMainValue( mGraph2->dataCount() - 1 );
	double graph3Value = mGraph3->dataMainValue( mGraph3->dataCount() - 1 );

	mTag1->updatePosition( graph1Value );
	mTag2->updatePosition( graph2Value );
	mTag3->updatePosition( graph3Value );

	mTag1->setText( QString::number( graph1Value, 'f', 2 ) );
	mTag2->setText( QString::number( graph2Value, 'f', 2 ) );
	mTag3->setText( QString::number( graph3Value, 'f', 2 ) );

	mPlot->replot();
}

void Archive::Run( void )
{
	mRun = !mRun;
	mRun ? mDataTimer.start( mplotTV ) : mDataTimer.stop();

	mRun ? uip->kledRunA->on() : uip->kledRunA->off();
	mRun ? uip->btnRunA->setText( "Stop" ) : uip->btnRunA->setText( "Run" );
}

