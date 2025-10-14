#ifndef VPLOT_H
#define VPLOT_H

#include <qframe.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpalette.h>
#include <qpushbutton.h>
#include "qcustomplot.h"
#include "axistag.h"
#include "addons.h"
#include <QSerialPort>
#include <QSerialPortInfo>

#include <mainwindow.h>

class VPlot: public QWidget
{
    Q_OBJECT

private:
    bool m_bBlink;
    double m_rangeX=200.0;

    QTimer mDataTimer;

    QSplitter *splPlot;
    QTextBrowser *ptxt;
    QTextBrowser *plog;
    QSplitter *spltxtview;
    WLabel *countpoint, *messages;

    double m_minVG0, m_maxVG0;
    //double m_minVG1, m_maxVG1;

    QPointer<QCPGraph> mGraph0;//, mGraph1, mGraph2, mGraph3;
    AxisTag *mTag0;//, *mTag1, *mTag2, *mTag3;
    QStringList mvalY;
    QFile *dataoutfile;
    QTextStream *dataoutstream;
    Ui_MainWindow *ui;

    void mPlotUpdate(void);
    void CreatePlotter(void);
    void Texttolog(const QString);

public:

    QCPItemText *textItem;
    QCustomPlot *mPlot;

    VPlot(QWidget *parent = NULL, Ui_MainWindow *UI = NULL);

    void MessageBox(const QString,const QColor &tcolor, const QColor &bcolor);
    void MessageLog(const QString);
    void SetData(const QString *);
    void SetRange(void);

    ~VPlot();
};
#endif // VPLOT_H

