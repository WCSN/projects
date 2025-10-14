#ifndef VPLOT_H
#define VPLOT_H

#include <QFrame>
#include <QLayout>
#include <QLabel>
#include <QPalette>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>

#include "qcustomplot.h"
#include "addons.h"
#include "axistag.h"
#include "mainwindow.h"

class VPlot: public QWidget
{
    Q_OBJECT

private:
    Ui_MainWindow *ui;
    MainWindow *mw;

    bool m_bBlink;
    double m_rangeX=20.0;

    QTimer mDataTimer;

    QSplitter *splInd,*splPlot,*spltxtview;
    QTextBrowser *plog;

    QPointer<QCPGraph> mGraph0;
    AxisTag *mTag0;
    QStringList mvalY;

    QString filedatasave;
    QFile *dataoutfile=nullptr;
    QTextStream *dataoutstream;
    QLabel *msMv, *msWl;

    void mPlotUpdate(void);
    void Datatocvs(const QString &);
    void CreateFileSave(void);
    void CloseFileSave(void);

private slots:
    void on_MouseMove(QMouseEvent *event);
    void on_MouseWheel(QWheelEvent *event);
    void on_MousePress(QMouseEvent *event);
    void on_MouseRelease(QMouseEvent *event);

public:
    double m_minVG0, m_maxVG0;
    double tmEX0 = 0.0;
    double tmEXN = 0.0;

    //QCPItemText *textItem;
    QCustomPlot *mPlot;
    QTextBrowser *ptxt;
    WLabel *countpoint, *messages;

    VPlot(QWidget *parent = nullptr, MainWindow *MW = nullptr, Ui_MainWindow *UI = nullptr);
    ~VPlot();

    void CreatePlotter(void);
    void MessageBox(const QString,const QColor &tcolor, const QColor &bcolor);
    void MessageLog(const QString);
    void PlotDataUpdate(const QString *);
    int counts(void);

    void SetRange(void);
    void initSettings(void);
    void ScaleFset(void);
    void CleanPlot(void);
};

#endif // VPLOT_H

