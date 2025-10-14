#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtCore>
#include <QtGui>
#include <QtMultimedia/QSound>
#include <QGridLayout>
#include <QSerialPort>
#include <QDesktopWidget>
#include <QDateTime>
#include <QPrinter>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
    class Options;
}
QT_END_NAMESPACE

class VPlot;
class Options;

struct Parameters
{
    //Serial Port
    QString serialPortName = QString("");
    QSerialPort::BaudRate    serialBaudRate    = QSerialPort::BaudRate::Baud9600;
    QSerialPort::DataBits    serialDataBits    = QSerialPort::DataBits::Data8;
    QSerialPort::Parity      serialParity      = QSerialPort::Parity::NoParity;
    QSerialPort::StopBits    serialStopBits    = QSerialPort::StopBits::OneStop;
    //QSerialPort::FlowControl serialFlowControl = QSerialPort::FlowControl::SoftwareControl;
    QSerialPort::FlowControl serialFlowControl = QSerialPort::FlowControl::NoFlowControl;
    int serialTimeOut = 10000;

    //Plotter
    double AxisX0 = 0;
    double AxisX0Size = 100;
    double AxisYmin = 4.50e-04;
    double AxisYmax = 9.00e-04;

    QString namedatafile = "brga.cvs";
    QString pathdata = "./data";
    QString pathlogs = "./log";
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void Run(void);
    void Receivedata(void); //serial port

    void on_lineEditFileDataSave_editingFinished();

    void on_lineEditAxX0_editingFinished();
    void on_lineEditAxXsize_editingFinished();
    void on_lineEditAxYmin_editingFinished();
    void on_lineEditAxYmax_editingFinished();

    void on_pushBOptions_clicked();
    void on_pushBScale0_clicked();
    void on_pushBScaleX_clicked();
    void on_pushBScaleY_clicked();
    void on_pushBClean_clicked();

public:
    Ui::MainWindow *ui;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool mRun = false;

    const Parameters defpar;
    Parameters defset;
    Parameters curset;

    QFont *mainFont;
    Options *opt = nullptr;
    VPlot *vplot = nullptr;

    QList<QSerialPortInfo> infoserialports;
    QSerialPort *serial = nullptr;;
    QLabel *SBarMesg, *SBarOGL, *SBarOGLst, *SBarPort, *SBarRate, *SBarPortSt, *SBarVers;

    void setSyncValue(int arg1);
    void setAutoNameFile(bool checked);
    void setOpenGLEbable(bool checked);

    void setcomboBoxPort(int index=0);
    void setcomboBoxRate(int index=0);
    void setAutoScale(void);
    void setAxixsPlotRange(double setAxisX0, double setAxisX0Size, double setAxYmin, double setAxYmax);

    void setParSerialPort(void);

    void loggedBuf(const QByteArray *buf);
    void loggedDmp(const QByteArray *dmp);
    void loggedDat(const QString *dat);

    void createLogs(void);
    void closeLogs(void);

    bool openSerialPort(void);
    void closeSerialPort(void);
    bool isMaxwin(void);

private:
    QTimer mSerialTimer;
    QGridLayout *layoutGlob = nullptr;
    QGridLayout *layoutPL = nullptr;

    QFile *fileLog = nullptr;
    QTextStream *streamLog = nullptr;

    QFile *fileDmp = nullptr;
    QDataStream *streamDmp = nullptr;

    QFile *fileBuf = nullptr;
    QDataStream *streamBuf = nullptr;

    void setEnableCnt(bool);
    void createStatusBar(void);
};

#endif // MAINWINDOW_H
