#include <QtGui>
#include <QtCore>
#include <math.h>

#include <qwt_analog_clock.h>
#include <qwt_round_scale_draw.h>
#include <time.h>

#include <kled.h>

#include "attitude_indicator.h"
#include "speedo_meter.h"
#include "volum_meter.h"
#include "clock.h"
#include "lcdind.h"
#include "addons.h"
#include "carbox.h"

#include "cockpit.h"

Cockpit::Cockpit( QWidget *parent, Ui_MainWindow * ui ) :
		QWidget(parent), settings("Wocson L", "br client"), uip(ui)
{
	QGridLayout *layoutGlob = new QGridLayout(parent);

	QWidget *areaL00 = new QWidget();
	QWidget *areaL01 = new QWidget();
	QWidget *areaL02 = new QWidget();
	QWidget *areaR01 = new QWidget();
	QWidget *areaR02 = new QWidget();

	splH = new QSplitter(Qt::Horizontal);
	splVL = new QSplitter(Qt::Vertical);
	splVR = new QSplitter(Qt::Vertical);

	QGridLayout *layoutL01 = new QGridLayout(areaL01);
	layoutL01->setMargin(0);
	layoutL01->setSpacing(5);

	QGridLayout *layoutL02 = new QGridLayout(areaL02);
	layoutL02->setMargin(0);
	layoutL02->setSpacing(5);

	QGridLayout *layoutR01 = new QGridLayout(areaR01);
	layoutR01->setMargin(0);
	layoutR01->setSpacing(5);

	QGridLayout *layoutR02 = new QGridLayout(areaR02);
	layoutR02->setMargin(0);
	layoutR02->setSpacing(5);

	mPlot = new QCustomPlot(areaL02);
	CreatePlotter();

	connect(&mDataTimer, SIGNAL(timeout()), this, SLOT(timerSlotmPlot()));
	connect(&timerS, SIGNAL(timeout()), this, SLOT(changeSpeed()));
	connect(&timerV, SIGNAL(timeout()), this, SLOT(changeVolum()));
	connect(&gradientTimer, SIGNAL(timeout()), this, SLOT(changeGradient()));
	connect(&angleTimer, SIGNAL(timeout()), this, SLOT(changeAngle()));

	uip->btnRunC->setParent(areaL00);
	uip->kledRunC->setParent(areaL00);

	CarBox *cbox = new CarBox(areaL00);
	cbox->setGeometry(75,0,200,40);

	for(int i = 0; i < 4; i++)
	{
		QwtDial *dial = createDial(i);
		layoutL01->addWidget(dial, 0, i);
	}

	layoutL02->addWidget(mPlot, 0, 0);

	layoutR01->addWidget(d_si, 1, 0);
	layoutR01->addWidget(d_vi, 2, 0);
	layoutR02->addWidget(d_ai, 3, 0);
	layoutR02->addWidget(d_gi, 4, 0);

	//for (i = 0; i < layoutL01->columnCount(); i++)
	//layoutL01->setColumnStretch(i, 1);

	splVL->insertWidget(0, areaL00);
	splVL->insertWidget(1, areaL01);
	splVL->insertWidget(2, areaL02);
	splVL->setStretchFactor(0, 2);
	splVL->setStretchFactor(1, 22);
	splVL->setStretchFactor(2, 130);

	splVR->insertWidget(0, areaR01);
	splVR->insertWidget(1, areaR02);
	splVR->setStretchFactor(0, 1);
	splVR->setStretchFactor(1, 1);

	splH->insertWidget(0, splVL);
	splH->insertWidget(1, splVR);
	splH->setStretchFactor(0, 2);
	splH->setStretchFactor(1, 2);

	layoutGlob->addWidget(splH);

	setFocus();
}

void Cockpit::CreatePlotter( void )
{
	// configure plot to have two right axes:
	mPlot->yAxis->setTickLabels(false);
	connect(mPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)), mPlot->yAxis, SLOT(setRange(QCPRange))); // left axis only mirrors inner right axis

	mPlot->yAxis2->setVisible(true);

	mPlot->axisRect()->addAxis(QCPAxis::atRight);
	mPlot->axisRect()->addAxis(QCPAxis::atRight);
	mPlot->axisRect()->addAxis(QCPAxis::atRight);

	mPlot->axisRect()->axis(QCPAxis::atRight, 0)->setPadding(10); // add some padding to have space for tags
	mPlot->axisRect()->axis(QCPAxis::atRight, 0)->setRange(0, 200);

	mPlot->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(10); // add some padding to have space for tags
	mPlot->axisRect()->axis(QCPAxis::atRight, 1)->setRange(0, 6500);

	mPlot->axisRect()->axis(QCPAxis::atRight, 2)->setPadding(10);
	mPlot->axisRect()->axis(QCPAxis::atRight, 2)->setRange(-180, 180);

	mPlot->axisRect()->axis(QCPAxis::atRight, 3)->setPadding(10);
	mPlot->axisRect()->axis(QCPAxis::atRight, 3)->setRange(-100, 100);

	// create graphs:
	mGraph1 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));

	mGraph2 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 1));
	mGraph3 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 2));
	mGraph4 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 3));

	mGraph1->setPen(QPen(QColor(0xAA, 0x00, 0x00)));
	mGraph2->setPen(QPen(QColor(0x00, 0xAA, 0x00)));
	mGraph3->setPen(QPen(QColor(0x00, 0x00, 0xAA)));
	mGraph4->setPen(QPen(QColor(0xAA, 0x00, 0xAA)));

	// create tags with newly introduced AxisTag class (see axistag.h/.cpp):
	mTag1 = new AxisTag(mGraph1->valueAxis());
	mTag1->setPen(mGraph1->pen());

	mTag2 = new AxisTag(mGraph2->valueAxis());
	mTag2->setPen(mGraph2->pen());

	mTag3 = new AxisTag(mGraph3->valueAxis());
	mTag3->setPen(mGraph3->pen());

	mTag4 = new AxisTag(mGraph4->valueAxis());
	mTag4->setPen(mGraph4->pen());
}

void Cockpit::timerSlotmPlot()
{
// calculate and add a new data point to each graph:
	mGraph1->addData(mGraph1->dataCount(), d_speedo->value());
	mGraph2->addData(mGraph2->dataCount(), d_volum->value() * 1000.0);

	double angle = d_al->angle();
	if(d_al->angle() >= 180.0 && d_al->angle() <= 360.0)
		angle = -(360.0 - d_al->angle());

	mGraph3->addData(mGraph3->dataCount(), angle);
	mGraph4->addData(mGraph4->dataCount(), d_al->gradient() * 100);

// make key axis range scroll with the data:
	mPlot->xAxis->rescale();

	mGraph1->setAntialiased(true);
	mGraph2->setAntialiased(true);
	mGraph3->setAntialiased(true);
	mGraph4->setAntialiased(true);

	mGraph1->rescaleValueAxis(true, true);
	mGraph2->rescaleValueAxis(true, true);
	mGraph3->rescaleValueAxis(true, true);
	mGraph4->rescaleValueAxis(true, true);

	mPlot->xAxis->setRange(mPlot->xAxis->range().upper, 200, Qt::AlignRight);

// update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
	double graph1Value = mGraph1->dataMainValue(mGraph1->dataCount() - 1);
	double graph2Value = mGraph2->dataMainValue(mGraph2->dataCount() - 1);
	double graph3Value = mGraph3->dataMainValue(mGraph3->dataCount() - 1);
	double graph4Value = mGraph4->dataMainValue(mGraph4->dataCount() - 1);

	mTag1->updatePosition(graph1Value);
	mTag2->updatePosition(graph2Value);
	mTag3->updatePosition(graph3Value);
	mTag4->updatePosition(graph4Value);

	mTag1->setText(QString::number(graph1Value, 'f', 2));
	mTag2->setText(QString::number(graph2Value, 'f', 2));
	mTag3->setText(QString::number(graph3Value, 'f', 2));
	mTag4->setText(QString::number(graph4Value, 'f', 2));

	mPlot->replot();
	setFocus();
}

void Cockpit::CarStop( void )
{
	timerS.stop();			//Speedo
	timerV.stop();			//Volum
	gradientTimer.stop();	//Altimeter
	angleTimer.stop();
	mDataTimer.stop();		//Plotter

	uip->kledRunC->off();
	uip->btnRunC->setText("Engine");
}

void Cockpit::Run( void )
{
	mRun = !mRun;

	if(mRun)
	{
		timerS.start(speedTV);			//Speedo
		timerV.start(volumTV);			//Volum
		gradientTimer.start(gradiTV);	//Altimeter
		angleTimer.start(angleTV);		//Altimeter
		mDataTimer.start(mplotTV);		//Plotter

		uip->kledRunC->on();
		uip->btnRunC->setText("Stop");
		setFocus();
	}
	else
	{
		d_volum->shift = -1;
	}
}

void Cockpit::writeSettings( const QString &nameset )
{
	QSettings settings("Wocson L", "BR Client");
	settings.beginGroup(nameset);
	settings.setValue("size", size());
	settings.setValue("splH", splH->saveState());
	settings.setValue("splVL", splVL->saveState());
	settings.setValue("splVR", splVR->saveState());
	settings.endGroup();
}

Cockpit::~Cockpit()
{
	writeSettings("cockpit");
}

void Cockpit::readSettings( const QString &nameset )
{
	QSettings settings("Wocson L", "BR Client");
	settings.beginGroup(nameset);
	resize(settings.value("size", QSize(480, 360)).toSize());

	QByteArray state = settings.value("splH").toByteArray();

	splH->restoreState(settings.value("splH").toByteArray());
	splVL->restoreState(settings.value("splVL").toByteArray());
	splVR->restoreState(settings.value("splVR").toByteArray());
	settings.endGroup();
}

void Cockpit::RestoreCurrentSettings( void )
{
	readSettings("cockpit");
}

void Cockpit::RestoreDefaultSettings( void )
{
	readSettings("cockpitdef");
}

void Cockpit::SaveDefaultSettings( void )
{
	writeSettings("cockpitdef");
}

QwtDial *Cockpit::createDial( int pos )
{
	QwtDial *dial = NULL;
	switch(pos)
	{
		case 0:
		{
			d_clock = new Clock(this);
			QTimer *timer = new QTimer(d_clock);
			timer->connect(timer, SIGNAL(timeout()), d_clock, SLOT(setCurrentTime()));
			timer->start(1000);
			dial = d_clock;

			break;
		}
		case 1:
		{
			d_speedo = new SpeedoMeter(this);
			d_speedo->setScaleStepSize(20.0);
			d_speedo->setScale(0.0, 180.0);
#ifdef __FreeBSD__
	        d_speedo->scaleDraw()->setPenWidth(2);
#else
	        d_speedo->scaleDraw()->setPenWidthF(2);
#endif
			d_si = new LCDIndicator(this, "Speed");
			dial = d_speedo;
			break;
		}
		case 2:
		{
			d_volum = new VolumMeter(this);
			d_vi = new LCDIndicator(this, "Volume");
			dial = d_volum;
			break;
		}

		case 3:
		{
			d_al = new AttitudeIndicator(this);
#ifdef __FreeBSD__
	        d_speedo->scaleDraw()->setPenWidth(3);
#else
	        d_speedo->scaleDraw()->setPenWidthF(3);
#endif
			d_ai = new LCDIndicator(this, "Angle");
			d_gi = new LCDIndicator(this, "Grad.");
			dial = d_al;
			break;
		}
	}

	if(dial)
	{
		dial->setReadOnly(true);
		dial->setLineWidth(5);
		dial->setFrameShadow(QwtDial::Sunken);
	}

	dial->setMaximumSize(220, 220);
	return dial;
}

void Cockpit::changeSpeed()
{
	double speed = d_speedo->value();
	double offset = 0.0;

	if(d_volum->value() <= 0.0)
	{
		d_speedo->setValue(0.0);
		d_si->setValue(0.0);
		return;
	}

	offset = (1 / log(d_volum->value() + 1)) * 1.5;

	if(d_volum->shift == -1)
		offset = -offset;
	else if(d_volum->shift == 777.0)
	{
		offset = 0.0;
		speed += randomW(-40, 40) / 100.0;
	}

	d_speedo->setValue(speed + offset);
	d_si->setValue(speed + offset);
}

void Cockpit::changeVolum()
{
	double offset = 0.0;
	double volum = d_volum->value();

	if(d_volum->value() <= 0.0 && !uip->btnRunC->isChecked())
		CarStop();

	if(volum > 5.5)
	{
		d_volum->setValue(5.5);
		return;
	}
	else if(volum <= 0.0)
	{
		d_volum->setValue(0.0);
		d_vi->setValue(d_volum->value() * 1000.0);
		return;
	}

	if(uip->btnRunC->isChecked())
		offset = (1 / log(volum + 1)) / 25;
	else
		offset = 0.2;

	if(d_volum->shift == -1)
		offset = -offset;
	else if(d_volum->shift == 777)
	{
		offset = 0;
		volum += randomW(-20, 20) / 1000;
	}

	d_volum->setValue(volum + offset);
	d_vi->setValue(d_volum->value() * 1000.0);

}

void Cockpit::changeAngle()
{
	double offset = 1;

	if(d_speedo->value() == 0.0)
	{
		d_al->setAngle(0.0);
		return;
	}

	double angle = d_al->angle() + randomW(-200, 200) / 100.0;

	if(angle == 0.0)
		return;
	else
		angleTimer.start(angleTV);

	if((angle >= 0.0 && angle <= 1.5) || (angle >= 358.5 && angle <= 359.9))
	{
		angle = 0.0;
		offset = 0.0;
	}
	else if(((angle >= 0.0 && angle <= 180.0) && offset > 0.0) || ((angle >= 180.0 && angle <= 360) && offset < 0.0))
		offset = -offset;

	d_al->setAngle(angle + offset);

	angle = angle + offset;
	if(angle >= 180.0 && angle <= 360.0)
		angle = -(360.0 - angle);

	d_ai->setValue(angle);
}

void Cockpit::changeGradient()
{
	double offset = 0.01;

	if(d_speedo->value() == 0.0)
	{
		d_al->setGradient(0.0);
		return;
	}

	double gradient = d_al->gradient() + randomW(-30, 30) / 1000;

	if(gradient == 0.0)
		return;
	else
		gradientTimer.start(gradiTV);

	if(gradient >= -0.02 && gradient <= 0.02)
	{
		gradient = 0.0;
		offset = 0.0;
	}
	else if((gradient < -0.02 && offset < 0.0) || (gradient > 0.02 && offset > 0.0))
		offset = -offset;

	d_al->setGradient(gradient + offset);
	d_gi->setValue(gradient);
}

void Cockpit::keyPressEvent( QKeyEvent *event )
{
	switch(event->key())
	{
		case Qt::Key_Plus:
		case Qt::Key_PageUp:
		{
			d_volum->shift = 1;
			d_volum->setValue(d_volum->value() + 0.1);
			break;
		}
		case Qt::Key_Minus:
		case Qt::Key_PageDown:
		{
			d_volum->shift = -1;
			d_volum->setValue(d_volum->value() - 0.1);
			break;
		}
		case Qt::Key_Asterisk:
		{
			d_volum->shift = 777;
			break;
		}
		case Qt::Key_Up:
		case Qt::Key_W:
		{
			d_al->setGradient(d_al->gradient() - 0.03);
			break;
		}
		case Qt::Key_Down:
		case Qt::Key_S:
		{
			d_al->setGradient(d_al->gradient() + 0.03);
			break;
		}
		case Qt::Key_Left:
		case Qt::Key_A:
		{
			d_al->setAngle(d_al->angle() + 2.0);
			break;
		}
		case Qt::Key_Right:
		case Qt::Key_D:
		{
			d_al->setAngle(d_al->angle() - 2.0);
			break;
		}

		default:
			QWidget::keyPressEvent(event);
	}
}

