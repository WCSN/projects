#include <QtGui>
#include <QFormLayout>
#include <qframe.h>
#include <qpalette.h>
#include "qcustomplot.h"
#include "axistag.h"
#include "addons.h"

#include "mainwindow.h"

class QwtDial;
class QwtAnalogClock;
class SpeedoMeter;
class AttitudeIndicator;
class VolumMeter;
class LCDIndicator;

class Cockpit: public QWidget
{
	Q_OBJECT

public:
	Cockpit(QWidget *parent = NULL,  Ui_MainWindow *ui = NULL);
	~Cockpit();

	//QPushButton *btnRunC;
	//KLed *kledRunC;

public Q_SLOTS:
	void SaveDefaultSettings(void);
	void RestoreCurrentSettings(void);

private Q_SLOTS:
	void changeSpeed();
	void changeVolum();
	void changeGradient();
	void changeAngle();

	void RestoreDefaultSettings(void);

	void timerSlotmPlot();
	void Run(void);

private:
	QwtDial *createDial(int pos);
	QwtAnalogClock *d_clock;
	SpeedoMeter *d_speedo;
	LCDIndicator *d_si;
	VolumMeter *d_volum;
	LCDIndicator *d_vi;
	AttitudeIndicator *d_al;
	LCDIndicator *d_ai;
	LCDIndicator *d_gi;

	QSplitter *splH, *splVL, *splVR;
	QSettings settings;

	QCustomPlot *mPlot;
	QPointer<QCPGraph> mGraph1;
	QPointer<QCPGraph> mGraph2;
	QPointer<QCPGraph> mGraph3;
	QPointer<QCPGraph> mGraph4;

	AxisTag *mTag1;
	AxisTag *mTag2;
	AxisTag *mTag3;
	AxisTag *mTag4;

	QTimer timerS;			uint speedTV=100;
	QTimer timerV;			uint volumTV=100;
	QTimer gradientTimer;	uint gradiTV=100;
	QTimer angleTimer;		uint angleTV=100;
	QTimer mDataTimer;		uint mplotTV=50;
	Ui_MainWindow *uip;

	bool mRun = false;
	void CreatePlotter(void);

	double offsetvol = -0.1;

private:
	void writeSettings(const QString &nameset);
	void readSettings(const QString &nameset);
	void CarStop(void);

protected:

	virtual void keyPressEvent( QKeyEvent * );
};
