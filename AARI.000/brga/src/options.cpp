#include <QFrame>
#include <QLayout>
#include <QLabel>
#include <QPalette>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QSplitter>
#include <QPrinter>
#include <QCheckBox>
#include <QTextBrowser>

#include "settings.h"
#include "vplot.h"
#include "options.h"

#include "ui_mainwindow.h"
#include "ui_options.h"

Options::Options(QWidget *parent, MainWindow *MW):
    QDialog(parent),
    uiopt(new Ui::Options),
    mw(MW)
{
    uiopt->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    initSerialPort();
    initSettings();

    addTexttoHelpReset();
}

Options::~Options()
{
    saveSettings();
    delete uiopt;
}

void Options::addTexttoHelpReset(void)
{
    uiopt->textBrowserNoteReset->append("X0 = " + QString().setNum((int)mw->defpar.AxisX0));
    uiopt->textBrowserNoteReset->append("XSize = " + QString().setNum((int)mw->defpar.AxisX0Size));
    uiopt->textBrowserNoteReset->append("Ymin = " + QString().setNum(mw->defpar.AxisYmin,'e',2));
    uiopt->textBrowserNoteReset->append("Ymax = " + QString().setNum(mw->defpar.AxisYmax,'e',2));

    uiopt->textBrowserNoteReset->append("\nSerial Port - install the first one found from the list");
    uiopt->textBrowserNoteReset->append("Serial Rate - " + QString().setNum((int)mw->defpar.serialBaudRate));
    uiopt->textBrowserNoteReset->append("Data bits - " + QString().setNum((int)mw->defpar.serialDataBits));
    uiopt->textBrowserNoteReset->append("Parity - None");
    uiopt->textBrowserNoteReset->append("Stop bits - " + QString().setNum((int)mw->defpar.serialStopBits));
    uiopt->textBrowserNoteReset->append("Flow control - None");
    uiopt->textBrowserNoteReset->append("Sync Time - " + QString().setNum((int)mw->defpar.serialTimeOut));
}

void Options::inituioptSerial(void)
{
    for (auto infoport : mw->infoserialports)
        uiopt->comboBoxPort->addItem(infoport.portName());

    uiopt->comboBoxPort->setCurrentIndex(0);

    for (auto brate : mw->infoserialports[0].standardBaudRates())
        uiopt->comboBoxRate->addItem(QString::number(brate),(QSerialPort::BaudRate) brate);

    uiopt->comboBoxRate->setCurrentIndex(uiopt->comboBoxRate->findText(QString::number(mw->curset.serialBaudRate)));

    // fill data bits
    uiopt->comboBoxDataBits->addItem(QLatin1String("5"), QSerialPort::Data5);
    uiopt->comboBoxDataBits->addItem(QLatin1String("6"), QSerialPort::Data6);
    uiopt->comboBoxDataBits->addItem(QLatin1String("7"), QSerialPort::Data7);
    uiopt->comboBoxDataBits->addItem(QLatin1String("8"), QSerialPort::Data8);

    // fill parity
    uiopt->comboBoxParity->addItem(QLatin1String("None"), QSerialPort::NoParity);
    uiopt->comboBoxParity->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    uiopt->comboBoxParity->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    uiopt->comboBoxParity->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    uiopt->comboBoxParity->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);

    // fill stop bits
    uiopt->comboBoxStopBits->addItem(QLatin1String("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    uiopt->comboBoxStopBits->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
#endif
    uiopt->comboBoxStopBits->addItem(QLatin1String("2"), QSerialPort::TwoStop);

    // fill flow control
    uiopt->comboBoxFlowControl->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    uiopt->comboBoxFlowControl->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    uiopt->comboBoxFlowControl->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);
}

void Options::initSerialPort(void)
{
    if (!mw->infoserialports.empty())
    {
        inituioptSerial();
    }
    else
    {
        uiopt->comboBoxPort->setEnabled(false);
        uiopt->comboBoxRate->setEnabled(false);
        mw->ui->btnStart->setEnabled(false);
        QString msg("Serial ports were not found in the system!");
        mw->SBarMesg->setText(msg);
        QPalette pal = palette();
        pal.setColor(QPalette::Window, Qt::gray);
        setAutoFillBackground(true);
        mw->ui->btnStart->setPalette(pal);
        mw->vplot->messageLog("Serial port not find");
    }
}

void Options::initSettings(void)
{

    mw->setGeometry(Settings::value("MWSize").toRect());

    // Defaults Set
    mw->defset.serialBaudRate = (QSerialPort::BaudRate) Settings::value("Default/Def_BaudRate", mw->defpar.serialBaudRate).toInt();
    mw->defset.serialTimeOut = Settings::value("Default/Def_SyncTimeOut", mw->defpar.serialTimeOut).toInt();

    mw->defset.AxisX0 = Settings::value("Default/Def_AxisX0",mw->defpar.AxisX0).toDouble();
    mw->defset.AxisX0Size = Settings::value("Default/Def_AxisX0Size", mw->defpar.AxisX0Size).toDouble();
    mw->defset.AxisYmin = Settings::value("Default/Def_AxYmin", mw->defpar.AxisYmin).toDouble();
    mw->defset.AxisYmax = Settings::value("Default/Def_AxYmax", mw->defpar.AxisYmax).toDouble();

    mw->curset.namedatafile = Settings::value("Data/NamefileDataSave", mw->defpar.namedatafile).toString();
    mw->curset.pathdata = Settings::value("Data/PathData", mw->defpar.pathdata).toString();
    mw->curset.pathlogs = Settings::value("Data/PathLogs", mw->defpar.pathlogs).toString();

    mw->ui->lineEditFileDataSave->setText(Settings::value("Data/NamefileDataSave", mw->curset.namedatafile).toString());
    uiopt->lineEditPathData->setText(Settings::value("Data/PathData", mw->curset.pathdata).toString());
    uiopt->lineEditPathLogs->setText(Settings::value("Data/PathLogs", mw->curset.pathlogs).toString());
    //

    // Plotter
    if (Settings::contains("AutoSaveSettings"))
        Settings::value("AutoSaveSettings").toString() == QString("Yes")
            ? uiopt->checkBAutoSaveSet->setCheckState(Qt::Checked)
            : uiopt->checkBAutoSaveSet->setCheckState(Qt::Unchecked);

    if (Settings::contains("FileAutoDataSave"))
        Settings::value("FileAutoDataSave").toString() == QString("Yes")
            ? uiopt->checkBAutoNameFile->setCheckState(Qt::Checked)
            : uiopt->checkBAutoNameFile->setCheckState(Qt::Unchecked);

    if (Settings::contains("LaunchMaximized"))
        Settings::value("LaunchMaximized").toString() == QString("Yes")
            ? uiopt->checkBLaunchMax->setCheckState(Qt::Checked)
            : uiopt->checkBLaunchMax->setCheckState(Qt::Unchecked);

    uiopt->lineEditAxX0->setText(Settings::value("Default/Def_AxisX0",mw->defset.AxisX0).toString());
    uiopt->lineEditAxXsize->setText(Settings::value("Default/Def_AxisXSize",mw->defset.AxisX0Size).toString());
    uiopt->lineEditAxYmin->setText(QString().setNum(Settings::value("Default/Def_AxisYMin",mw->defset.AxisYmin).toDouble(),'e',2));
    uiopt->lineEditAxYmax->setText(QString().setNum(Settings::value("Default/Def_AxisYMax",mw->defset.AxisYmax).toDouble(),'e',2));

    mw->ui->lineEditAxX0->setText(Settings::value("Plotter/AxisX0",mw->defset.AxisX0).toString());
    mw->ui->lineEditAxXsize->setText(Settings::value("Plotter/AxisXSize",mw->defset.AxisX0Size).toString());
    mw->ui->lineEditAxYmin->setText(QString().setNum(Settings::value("Plotter/AxisYMin", mw->defset.AxisYmin).toDouble(),'e',2));
    mw->ui->lineEditAxYmax->setText(QString().setNum(Settings::value("Plotter/AxisYMax", mw->defset.AxisYmax).toDouble(),'e',2));


    if (Settings::value("Plotter/AutoScale").toString() == QString("Yes"))
    {
        uiopt->checkBAutoScaleY->setCheckState(Qt::Checked);
        mw->ui->pushBScaleY->setEnabled(false);
    }
    else
    {
        uiopt->checkBAutoScaleY->setCheckState(Qt::Unchecked);
        mw->ui->pushBScaleY->setEnabled(true);
    }

    Settings::value("Plotter/OpenGL").toString() == QString("Yes")
        ? uiopt->checkBOGL->setCheckState(Qt::Checked)
        : uiopt->checkBOGL->setCheckState(Qt::Unchecked);

    // SerialPort
    mw->curset.serialBaudRate = (QSerialPort::BaudRate) Settings::value("Serial/BaudRate", mw->defpar.serialBaudRate).toInt();

    int index = uiopt->comboBoxRate->findText(Settings::value("Serial/BaudRate", mw->defpar.serialBaudRate).toString());
    if (index != -1)
        uiopt->comboBoxRate->setCurrentIndex(index);
    else
    {
        index = uiopt->comboBoxRate->findData(mw->defpar.serialBaudRate);
        if (index != -1)
            uiopt->comboBoxRate->setCurrentIndex(index);
        else
        {
            uiopt->comboBoxRate->setCurrentText(QString::number(mw->defpar.serialBaudRate));
            uiopt->comboBoxRate->setItemData(uiopt->comboBoxRate->currentIndex(), (QSerialPort::BaudRate) mw->defpar.serialBaudRate);
        }
    }

    uiopt->comboBoxDataBits->setCurrentIndex(Settings::value("Serial/DataBits","3").toInt());
    uiopt->comboBoxParity->setCurrentIndex(Settings::value("Serial/Parity","0").toInt());
    uiopt->comboBoxStopBits->setCurrentIndex(Settings::value("Serial/StopBits","0").toInt());
    uiopt->comboBoxFlowControl->setCurrentIndex(Settings::value("Serial/FlowControl",mw->defpar.serialFlowControl).toInt());
    uiopt->spinBoxSync->setValue(Settings::value("Serial/SyncTimeOut",mw->defset.serialTimeOut).toInt());

    mw->SBarPort->setText(Settings::value("Serial/Port",uiopt->comboBoxPort->currentText()).toString());
    mw->SBarRate->setText(Settings::value("Serial/BaudRate",uiopt->comboBoxRate->currentData().toInt()).toString());

    mw->ui->lineEditFileDataSave->setEnabled(uiopt->checkBAutoNameFile->isChecked());

    saveSettings();
}

void Options::saveSettings(void)
{
    Settings::clear();
/*
 *  QScreen *screen = QGuiApplication::primaryScreen();
    auto pwgt = screen->availableGeometry() ;

    auto const rec = QApplication::desktop()->screenGeometry();
    auto const height = rec.height();
    auto const width = rec.width();
*/
    if(!uiopt->checkBLaunchMax->isChecked())
        Settings::setValue("MWSize", mw->geometry());

    Settings::setValue("AutoSaveSettings", uiopt->checkBAutoSaveSet->isChecked() ? "Yes" : "No");
    Settings::setValue("FileAutoDataSave", uiopt->checkBAutoNameFile->isChecked() ? "Yes" : "No");
    Settings::setValue("LaunchMaximized", uiopt->checkBLaunchMax->isChecked() ? "Yes" : "No");

    Settings::setValue("Data/NamefileDataSave", mw->ui->lineEditFileDataSave->text());
    Settings::setValue("Data/PathData", uiopt->lineEditPathData->text());
    Settings::setValue("Data/PathLogs", uiopt->lineEditPathLogs->text());

    Settings::setValue("Plotter/AxisX0", QString::number((int)mw->vplot->m_rangeX0));
    Settings::setValue("Plotter/AxisXSize", QString::number((int)mw->vplot->m_rangeXS));
    Settings::setValue("Plotter/AxisYMin", QString::number(mw->vplot->m_minVG0,'e',2));
    Settings::setValue("Plotter/AxisYMax", QString::number(mw->vplot->m_maxVG0,'e',2));
    Settings::setValue("Plotter/AutoScale", uiopt->checkBAutoScaleY->isChecked() ? "Yes" : "No");
    Settings::setValue("Plotter/OpenGL", uiopt->checkBOGL->isChecked() ? "Yes" : "No");

    Settings::setValue("Serial/Port", uiopt->comboBoxPort->currentText());
    Settings::setValue("Serial/BaudRate", uiopt->comboBoxRate->currentData().toInt());

    Settings::setValue("Serial/DataBits", uiopt->comboBoxDataBits->currentIndex());
    Settings::setValue("Serial/Parity", uiopt->comboBoxParity->currentIndex());
    Settings::setValue("Serial/StopBits", uiopt->comboBoxStopBits->currentIndex());
    Settings::setValue("Serial/FlowControl", uiopt->comboBoxFlowControl->currentIndex());
    Settings::setValue("Serial/SyncTimeOut", uiopt->spinBoxSync->value());

    Settings::setValue("Default/Def_AxisX0", uiopt->lineEditAxX0->text());
    Settings::setValue("Default/Def_AxisX0Size", uiopt->lineEditAxXsize->text());
    Settings::setValue("Default/Def_AxYmin", uiopt->lineEditAxYmin->text());
    Settings::setValue("Default/Def_AxYmax", uiopt->lineEditAxYmax->text());
    Settings::setValue("Default/Def_BaudRate", uiopt->comboBoxRate->currentData().toInt());
    Settings::setValue("Default/Def_SyncTimeOut", uiopt->spinBoxSync->value());
}

void Options::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter p(this);
    QStyle* style = this->style();
    QRect active_area = this->rect();
    int titlebar_height = 0;

    // Titlebar.
    QStyleOptionTitleBar t_opt;
    t_opt.initFrom(this);

    titlebar_height = style->pixelMetric(QStyle::PM_TitleBarHeight, &t_opt, this);

    t_opt.rect = QRect(0, 0, this->width(), titlebar_height);
    t_opt.titleBarState = this->windowState();
    t_opt.text = t_opt.fontMetrics.elidedText(this->windowTitle(), Qt::ElideRight, t_opt.rect.width());
    style->drawComplexControl(QStyle::CC_TitleBar, &t_opt, &p, this);
    style->drawItemText(&p, t_opt.rect, Qt::AlignCenter, t_opt.palette, true, t_opt.text, QPalette::ToolTipText);

    // Background widget.
    active_area.setTopLeft(QPoint(0, titlebar_height));
    this->setContentsMargins(0, titlebar_height, 0, 0);

    QStyleOption w_opt;
    w_opt.initFrom(this);
    w_opt.rect = active_area;
    style->drawPrimitive(QStyle::PE_Widget, &w_opt, &p, this);
}

void Options::saveBefore(void)
{
    optsave.state_checAutoNameFile  = uiopt->checkBAutoNameFile->isChecked();
    optsave.state_checkBAutoScaleY  = uiopt->checkBAutoScaleY->isChecked();
    optsave.state_checkBOGL         = uiopt->checkBOGL->isChecked();
    optsave.state_checkBAutoSaveSet = uiopt->checkBAutoSaveSet->isChecked();
    optsave.state_checkBLaunchMax   = uiopt->checkBLaunchMax->isChecked();

    optsave.PortName = uiopt->comboBoxPort->currentText();
    optsave.BaudRateText = uiopt->comboBoxRate->currentText();
    optsave.BaudRateData = uiopt->comboBoxRate->currentData();
    optsave.DataBitsText = uiopt->comboBoxDataBits->currentText();
    optsave.DataBitsData = uiopt->comboBoxDataBits->currentData();
    optsave.ParityText = uiopt->comboBoxParity->currentText();
    optsave.ParityData = uiopt->comboBoxParity->currentData();
    optsave.StopBitsText = uiopt->comboBoxStopBits->currentText();
    optsave.StopBitsData = uiopt->comboBoxStopBits->currentData().toInt();
    optsave.FlowControlText = uiopt->comboBoxFlowControl->currentText();
    optsave.FlowControlData = uiopt->comboBoxFlowControl->currentData();
    optsave.SyncTimeOut = uiopt->spinBoxSync->value();

    optsave.AxisX0     = uiopt->lineEditAxX0->text().toDouble();
    optsave.AxisX0Size = uiopt->lineEditAxXsize->text().toDouble();;
    optsave.AxisYmin   = uiopt->lineEditAxYmin->text().toDouble();
    optsave.AxisYmax   = uiopt->lineEditAxYmax->text().toDouble();

    optsave.pathdata   = uiopt->lineEditPathData->text();
    optsave.pathlogs   = uiopt->lineEditPathLogs->text();
}

void Options::restoreBefore(void)
{
    uiopt->checkBAutoNameFile->setChecked(optsave.state_checAutoNameFile);
    uiopt->checkBAutoScaleY->setChecked(optsave.state_checkBAutoScaleY);
    uiopt->checkBOGL->setChecked(optsave.state_checkBOGL);
    uiopt->checkBAutoSaveSet->setChecked(optsave.state_checkBAutoSaveSet);
    uiopt->checkBLaunchMax->setChecked(optsave.state_checkBLaunchMax);

    uiopt->comboBoxPort->setCurrentText(optsave.PortName.toString());
    uiopt->comboBoxRate->setCurrentText(optsave.BaudRateText.toString());
    uiopt->comboBoxRate->setItemData(uiopt->comboBoxRate->currentIndex(), optsave.BaudRateData);
    uiopt->comboBoxDataBits->setCurrentText(optsave.DataBitsText.toString());
    uiopt->comboBoxDataBits->setItemData(uiopt->comboBoxDataBits->currentIndex(),optsave.DataBitsData.toString());
    uiopt->comboBoxParity->setCurrentText(optsave.ParityText.toString());
    uiopt->comboBoxParity->setItemData(uiopt->comboBoxParity->currentIndex(),optsave.ParityData.toString());
    uiopt->comboBoxStopBits->setCurrentText(optsave.StopBitsText.toString());
    uiopt->comboBoxStopBits->setItemData(uiopt->comboBoxStopBits->currentIndex(),optsave.StopBitsData);
    uiopt->comboBoxFlowControl->setCurrentText(optsave.FlowControlText.toString());
    uiopt->comboBoxFlowControl->setItemData(uiopt->comboBoxFlowControl->currentIndex(),optsave.FlowControlData);

    uiopt->lineEditAxX0->setText(QString().setNum((int) optsave.AxisX0));
    uiopt->lineEditAxXsize->setText(QString().setNum((int) optsave.AxisX0Size));
    uiopt->lineEditAxYmin->setText(QString().setNum(optsave.AxisYmin, 'e', 2));
    uiopt->lineEditAxYmax->setText(QString().setNum(optsave.AxisYmax, 'e', 2));

    uiopt->lineEditPathData->setText(optsave.pathdata);
    uiopt->lineEditPathLogs->setText(optsave.pathlogs);
}

void Options::on_pushBOk_clicked()
{
    saveSettings();
    close();
}

void Options::on_pushBCancel_clicked()
{
    restoreBefore();
    close();
}

void Options::on_spinBoxSync_valueChanged(int arg1)
{
    mw->setSyncValue(arg1);
}

void Options::on_checkBAutoSaveSet_clicked(bool checked)
{
    if(checked)
        saveSettings();
}

void Options::on_checkBAutoNameFile_clicked(bool checked)
{
    mw->setAutoNameFile(checked);
}

void Options::on_checkBAutoScaleY_clicked()
{
    mw->setAutoScale();
}

void Options::on_checkBOGL_clicked(bool checked)
{
    mw->setOpenGLEbable(checked);
}

void Options::on_comboBoxPort_activated(int index)
{
    mw->setcomboBoxPort(index);
}

void Options::on_comboBoxRate_activated(int index)
{
    mw->setcomboBoxRate(index);
}

void Options::on_pushBSerialPortDefSet_clicked()
{
    uiopt->comboBoxPort->setCurrentText(mw->defpar.serialPortName);
    uiopt->comboBoxRate->setCurrentText(QString().setNum((QSerialPort::BaudRate)mw->defpar.serialBaudRate));
    uiopt->comboBoxDataBits->setCurrentText(QString().setNum((QSerialPort::DataBits)mw->defpar.serialDataBits));
    uiopt->comboBoxParity->setCurrentIndex(mw->defpar.serialParity);
    uiopt->comboBoxStopBits->setCurrentText(QString().setNum((QSerialPort::StopBits)mw->defpar.serialStopBits));
    uiopt->comboBoxFlowControl->setCurrentIndex(mw->defpar.serialFlowControl);
    uiopt->spinBoxSync->setValue(mw->defpar.serialTimeOut);
}

void Options::setAxixsPlotRange(double AxisX0,double AxisX0Size,double AxisYmin,double AxisYmax)
{
    uiopt->lineEditAxX0->setText(QString().setNum((int)AxisX0));
    uiopt->lineEditAxXsize->setText(QString().setNum((int)AxisX0Size));
    uiopt->lineEditAxYmin->setText(QString().setNum(AxisYmin,'e',2));
    uiopt->lineEditAxYmax->setText(QString().setNum(AxisYmax,'e',2));
    mw->ui->pushBScaleY->setEnabled(true);
}

void Options::on_pushBPlotDefReset_clicked()
{
    uiopt->lineEditAxX0->setText(QString().setNum(mw->defpar.AxisX0));
    uiopt->lineEditAxXsize->setText(QString().setNum(mw->defpar.AxisX0Size));
    uiopt->lineEditAxYmin->setText(QString().setNum(mw->defpar.AxisYmin, 'e', 2));
    uiopt->lineEditAxYmax->setText(QString().setNum(mw->defpar.AxisYmax, 'e', 2));

    setAxixsPlotRange(mw->defpar.AxisX0,mw->defpar.AxisX0Size,mw->defpar.AxisYmin,mw->defpar.AxisYmax);
}

void Options::on_pushBPlotDefSave_clicked()
{
    mw->defset.AxisX0 = uiopt->lineEditAxX0->text().toDouble();
    mw->defset.AxisX0Size = uiopt->lineEditAxXsize->text().toDouble();
    mw->defset.AxisYmin = uiopt->lineEditAxYmin->text().toDouble();
    mw->defset.AxisYmax = uiopt->lineEditAxYmax->text().toDouble();

    setAxixsPlotRange(mw->defset.AxisX0,mw->defset.AxisX0Size,mw->defset.AxisYmin,mw->defset.AxisYmax);


    if(uiopt->checkBAutoScaleY->isChecked())
        mw->setAxixsPlotRange(mw->defset.AxisX0,mw->defset.AxisX0Size,mw->defset.AxisYmin,mw->defset.AxisYmax);

    saveSettings();
}

void Options::on_lineEditPathLogs_editingFinished()
{
    mw->closeLogs();
    mw->curset.pathlogs = uiopt->lineEditPathLogs->text();
    mw->createLogs();
}

void Options::on_lineEditPathData_editingFinished()
{
    mw->curset.pathdata = uiopt->lineEditPathData->text();
}

void Options::on_pushBPathData_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Get path to Data", "./");

    if (!path.isEmpty())
        uiopt->lineEditPathData->setText(QString("%1").arg(path));
}

void Options::on_pushBPathLogs_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Get path to Logs", "./");

    if (!path.isEmpty())
    {
        uiopt->lineEditPathLogs->setText(QString("%1").arg(path));
        on_lineEditPathLogs_editingFinished();
    }
}

void Options::on_pushBSetAllDef_clicked()
{
    on_pushBPlotDefReset_clicked();
    on_pushBSerialPortDefSet_clicked();

    uiopt->lineEditPathData->setText(mw->defpar.pathdata);
    uiopt->lineEditPathLogs->setText(mw->defpar.pathlogs);

    uiopt->checkBAutoSaveSet->setChecked(optdef.state_checkBAutoSaveSet);
    uiopt->checkBAutoNameFile->setChecked(optdef.state_checAutoNameFile);
    mw->setAutoNameFile(optdef.state_checAutoNameFile);
    uiopt->checkBAutoScaleY->setChecked(optdef.state_checkBAutoScaleY);
    uiopt->checkBOGL->setChecked(optdef.state_checkBOGL);
    uiopt->checkBLaunchMax->setChecked(optdef.state_checkBLaunchMax);
}
