#include <qframe.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpalette.h>
#include <qpushbutton.h>
#include "qcustomplot.h"
#include "axistag.h"
#include "addons.h"

#include "mainwindow.h"

class Archive: public QWidget
{
	Q_OBJECT

public:
	Archive(QWidget *parent = NULL, Ui_MainWindow *ui = NULL);

private Q_SLOTS:

	void mPlotSlot();
	void Run(void);

private:


	WLabel *countpoint, *messages;

	QCustomPlot *mPlot;
	QPointer<QCPGraph> mGraph1, mGraph2, mGraph3;

	AxisTag *mTag1, *mTag2, *mTag3;

	QTimer mDataTimer;	uint mplotTV = 50;
	bool mRun = false;

	Ui_MainWindow *uip;



	void CreatePlotter(void);
};
