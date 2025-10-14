#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QSerialPort>
#include "QVariant"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
    class Options;
}
QT_END_NAMESPACE

class MainWindow;

struct OptParam
{
    // checkBox
    bool state_checAutoNameFile = true;
    bool state_checkBAutoScaleY = true;
    bool state_checkBAutoSaveSet = true;
    bool state_checkBLaunchMax = true;
    bool state_checkBOGL = false;

    // Serial Port
    QVariant PortName;
    QVariant BaudRateText;
    QVariant BaudRateData;
    QVariant DataBitsText;
    QVariant DataBitsData;
    QVariant ParityText;
    QVariant ParityData;
    QVariant StopBitsText;
    QVariant StopBitsData;
    QVariant FlowControlText;
    QVariant FlowControlData;

    int SyncTimeOut = 10000;

    // Plotter
    double AxisX0 = 0;
    double AxisX0Size = 100;
    double AxisYmin = 4.50e-04;
    double AxisYmax = 9.00e-04;

    QString pathdata;
    QString pathlogs;
};

class Options : public QDialog
{
    Q_OBJECT

public:
    Ui::Options *uiopt = nullptr;
    MainWindow *mw = nullptr;

    explicit Options(QWidget *parent = nullptr,MainWindow *MW = nullptr);
    ~Options();

    void saveBefore(void);
    void restoreBefore(void);
    void saveSettings(void);

private slots:

    void paintEvent(QPaintEvent* event);

    void on_pushBCancel_clicked();
    void on_pushBOk_clicked();
    void on_pushBPlotDefReset_clicked();
    void on_pushBSerialPortDefSet_clicked();
    void on_pushBPlotDefSave_clicked();
    void on_pushBPathData_clicked();
    void on_pushBPathLogs_clicked();
    void on_pushBSetAllDef_clicked();

    void on_checkBAutoNameFile_clicked(bool checked);
    void on_checkBAutoScaleY_clicked();
    void on_checkBOGL_clicked(bool checked);
    void on_checkBAutoSaveSet_clicked(bool checked);

    void on_spinBoxSync_valueChanged(int arg1);
    void on_comboBoxPort_activated(int index);
    void on_comboBoxRate_activated(int index);

    void on_lineEditPathLogs_editingFinished();
    void on_lineEditPathData_editingFinished();

private:
    OptParam optdef;
    OptParam optsave;

    void initSerialPort(void);
    void initSettings(void);
    void inituioptSerial(void);
    void addTexttoHelpReset(void);
    void setAxixsPlotRange(double AxisX0, double AxisX0Size, double AxisYmin, double AxisYmax);
};

#endif // OPTIONS_H
