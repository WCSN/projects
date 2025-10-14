#ifndef VPLOT_H
#define VPLOT_H

#include "axistag.h"
#include "mainwindow.h"
#include "qcustomplot.h"
#include "plotterbox.h"
#include "qobjectdefs.h"

class VPlot: public QWidget
{
    Q_OBJECT

private:
    MainWindow *mw;

    bool m_bBlink;
    bool Press = false;
    bool Move = false;

    QTimer mDataTimer;
    QFont* fontVPlot;

    QSplitter* splPlot;

    QPointer<QCPGraph> mGraph0;
    AxisTag *mTag0;
    QLabel *msMv, *msWl;

    QString filedatasave;
    QFile *fileData = nullptr;
    QTextStream *streamData = nullptr;

    QCPGraph *mMPoints = nullptr;
    QCPGraph *mRPoints = nullptr;
    QCPGraph *mWPoints = nullptr;

    void mPlotUpdate(double yV);

    void datatocvs(const QString &);
    void showCoord(void);
    void setScaleF(void);
    void point(QCPGraph*,int x, int y, bool setPt=false, const QColor &color=QColor(Qt::green));
    void setPoint(QMouseEvent *event, QCPGraph*, bool setPt=false, const QColor &color=QColor(Qt::green));
    void setPoint(QWheelEvent *event, QCPGraph*, bool setPt=false, const QColor &color=QColor(Qt::green));
    void savePlotGraph(const QString &fileName, const QString &ext);
    void initsavePlotBox(void);

    QCPAxis *axisXB0, *axisXT0;
    QCPAxis *axisYL0, *axisYR0;

    double tmX0=0.0;
    void setAxis_X(QCPAxis *axis);
    void setAxis_Y(QCPAxis *axis);
    void setaxisGrid(QCPAxis *axisx);
    void createPdf();

private slots:
    void on_MouseMove(QMouseEvent *event);
    void on_MouseWheel(QWheelEvent *event);
    void on_MousePress(QMouseEvent *event);
    void on_MouseRelease(QMouseEvent *event);
    void on_pushBSavePlot_release(void);

    void doPrintPreview();
    void printPreview(QPrinter *printer);

public:
    double m_rangeX0;
    double m_rangeXS;
    double m_minVG0;
    double m_maxVG0;

private:
    double m_rangeX1;
    QString mData;
    void Datafilter(QByteArray* buffer);

public:
    explicit VPlot(QWidget *parent = nullptr, MainWindow *MW = nullptr);
    ~VPlot();

    QCustomPlot *mPlot = nullptr;
    PlotterBox *mngViewBox = nullptr;
    //WLabel *counter, *serialstatus;

    void CreatePlotter(void);
    void messageLog(const QString, bool timest=false);
    void setDataPlot(QByteArray *buf);

    int counts(void);
    bool isDataHave(void);

    void initSettings(bool OGL=false);
    void cleanPlot(bool fullclean=true);
    void setRange(void);
    void setRangeX(bool replot=false);
    void setRangeY(bool replot=false);
    void setSavePlotBoxEnabled(void);
    void setOpenGL(bool opengl=true);
    void createFileSave(bool fullclean=false);
    void closeFileSave(void);
};

#endif // VPLOT_H

