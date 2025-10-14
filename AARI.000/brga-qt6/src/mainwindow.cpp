#include "mainwindow.h"
#include "vplot.h"
#include "settings.h"
#include "addons.h"

static const QString version("0.7.17b(qt6)");

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    infoserialports(QSerialPortInfo::availablePorts()),
    serial(new QSerialPort)
{
    ui->setupUi(this);

    m_chserialrate = m_serialdefrate;

    if(!infoserialports.empty())
    {
        for ( const QSerialPortInfo &infoport : infoserialports)
            ui->comboBoxPort->addItem(infoport.portName());

        for (auto &brate : infoserialports[0].standardBaudRates())
            ui->comboBoxRate->addItem(QString::number(brate));
    }
    else
    {
        ui->comboBoxPort->addItem("None");
        ui->comboBoxRate->addItem("0");
    }

    QScreen *screen = QGuiApplication::primaryScreen();

    pwgt = screen->availableGeometry() ;

    this->setGeometry(0, 0, pwgt.width(), pwgt.height());

    vplot = new VPlot(parent, this, ui);

    createStatusBar();
    InitSettings();

    connect(ui->btnStart, SIGNAL(released()), this, SLOT(Run()));
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::Receivedata);
}

MainWindow::~MainWindow()
{
    if (ui->checkBAutoSaveSet->isChecked()) SaveSettings();
    if( serial->isOpen() ) closeSerialPort();
    delete ui;
}

void MainWindow::createStatusBar(void)
{
    QFrame::Shape shape = QFrame::Shape::NoFrame;

    SMesg = new QLabel("",this);
    SMesg->setAlignment(Qt::AlignLeft);
    SMesg->setFrameShape(shape);

    SPort = new QLabel("", this);
    SPort->setAlignment(Qt::AlignLeft);
    SPort->setFrameShape(shape);
    SPort->setMinimumSize(SPort->sizeHint());

    SRate = new QLabel("", this);
    SRate->setAlignment(Qt::AlignRight);
    SRate->setFrameShape(shape);
    SRate->setMinimumSize(SRate->sizeHint());

    SVers = new QLabel("v."+version, this);
    SVers->setAlignment(Qt::AlignRight);
    SVers->setFrameShape(shape);
    SVers->setMinimumSize(SVers->sizeHint());

    statusBar()->addWidget(SMesg,1);
    statusBar()->addWidget(SPort);
    statusBar()->addWidget(SRate);
    statusBar()->addWidget(SVers);
}

void MainWindow::InitSettings(void)
{
    if (Settings::contains("Serial/Rate"))
        ui->comboBoxRate->setCurrentText(Settings::value("Serial/Rate").toString());
    else
        ui->comboBoxRate->setCurrentText("9600");

    m_chserialrate = Settings::value("Serial/Rate").toInt();
    ui->spinBoxSync->setRange(1,5000);

    (Settings::contains("Data/FileNameDataSave")) ? ui->lineEditFileDataSave->setText(Settings::value("Data/FileNameDataSave").toString()) : ui->lineEditFileDataSave->setText("brga-" + CurrentTime("yyyyMMdd-hhmmss") + ".csv");

    if(Settings::contains("Serial/Synchrotime"))
        ui->spinBoxSync->setValue(Settings::value("Serial/Synchrotime").toInt());
    else
        ui->spinBoxSync->setValue(mSerialTimeOut);

    (Settings::contains("Plotter/AxisYMin")) ? ui->lineEditAxYMin->setText(Settings::value("Plotter/AxisYMin").toString()) : ui->lineEditAxYMin->setText("4.50e-4");
    (Settings::contains("Plotter/AxisYMax")) ? ui->lineEditAxYMax->setText(Settings::value("Plotter/AxisYMax").toString()) : ui->lineEditAxYMax->setText("9.00e-4");

    (Settings::value("AutoSave").toString() == QString("Yes")) ? ui->checkBAutoSaveSet->setCheckState(Qt::Checked) : ui->checkBAutoSaveSet->setCheckState(Qt::Unchecked);
    (Settings::value("FileAutoDataSave").toString() == QString("Yes")) ? ui->checkBAutoNameFile->setCheckState(Qt::Checked) : ui->checkBAutoNameFile->setCheckState(Qt::Unchecked);
    (Settings::value("AutoScale").toString() == QString("Yes")) ? ui->checkBAutoScale->setCheckState(Qt::Checked) : ui->checkBAutoScale->setCheckState(Qt::Unchecked);

    (ui->checkBAutoNameFile->isChecked()) ? ui->lineEditFileDataSave->setEnabled(false) : ui->lineEditFileDataSave->setEnabled(true);

    SaveSettings();

    vplot->initSettings();
    vplot->SetRange();

    SPort->setText(Settings::value("Serial/Port").toString());
    SRate->setText(Settings::value("Serial/Rate").toString());
}

void MainWindow::MessageBox(const QString message,const QColor &tcolor, const QColor &bcolor)
{
    vplot->messages->wsetColors(tcolor, bcolor);
    vplot->messages->wsetText(message);
    SMesg->setText(message);
}

void MainWindow::CountBox(const QString count,const QColor &tcolor, const QColor &bcolor)
{
    vplot->countpoint->wsetColors(tcolor, bcolor);
    vplot->countpoint->wsetText(count);
}

void MainWindow::SaveSettings(void)
{
    Settings::clear();
    Settings::setValue("Serial/Port", ui->comboBoxPort->currentText());
    Settings::setValue("Serial/Rate", ui->comboBoxRate->currentText());
    Settings::setValue("Serial/Synchrotime", ui->spinBoxSync->value());
    Settings::setValue("Plotter/AxisYMin", QString::number(vplot->m_minVG0,'e',2));
    Settings::setValue("Plotter/AxisYMax", QString::number(vplot->m_maxVG0,'e',2));
    Settings::setValue("Data/FileNameDataSave", ui->lineEditFileDataSave->text());
    Settings::setValue("FileAutoDataSave", ui->checkBAutoNameFile->isChecked() ? "Yes" : "No");
    Settings::setValue("AutoSave", ui->checkBAutoSaveSet->isChecked() ? "Yes" : "No");
    Settings::setValue("AutoScale", ui->checkBAutoScale->isChecked() ? "Yes" : "No");
}

void MainWindow::enabledpointset(bool state)
{
    ui->lineEditFileDataSave->setEnabled(state);
    ui->comboBoxPort->setEnabled(state);
    ui->comboBoxRate->setEnabled(state);

    (ui->checkBAutoNameFile->isChecked()) ? ui->lineEditFileDataSave->setEnabled(false) : ui->lineEditFileDataSave->setEnabled(true);
}

void MainWindow::openSerialPort(void)
{
    if( serial->isOpen() )
        closeSerialPort();

    QString serialportname(ui->comboBoxPort->currentText());

    serial->setPortName(serialportname);
    if ( serial->open(QIODevice::ReadOnly) )
    {
        serial->setBaudRate(m_chserialrate);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        MessageBox("Data received",Qt::green, Qt::black);
        vplot->MessageLog("Connect to: "+serialportname+", speed: "+QString::number(serial->baudRate()));
        vplot->MessageLog("Receiving data");
        ui->btnStart->setText( "Stop" );
    }
    else
    {
        MessageBox(serial->errorString(),Qt::red, Qt::black);
        ui->btnStart->setText( "Start" );
        MessageBox("Disconnect",Qt::red, Qt::black);
        vplot->MessageLog("Disconnect");
        enabledpointset(true);
    }
}

void MainWindow::closeSerialPort(void)
{
    serial->clear();
    serial->close();
}

void MainWindow::on_comboBoxPort_activated(int index)
{
    if( serial->isOpen() ) closeSerialPort();

    SPort->setText(ui->comboBoxPort->currentText());

    ui->comboBoxRate->clear();

    for (auto &brate : infoserialports[index].standardBaudRates())
        ui->comboBoxRate->addItem(QString::number(brate));

    ui->comboBoxRate->setCurrentIndex(ui->comboBoxRate->findText(QString::number(m_chserialrate)));
    if (ui->checkBAutoSaveSet->isChecked()) SaveSettings();
}

void MainWindow::on_comboBoxRate_activated(int index)
{
    m_chserialrate = ui->comboBoxRate->currentText().toInt();
    SRate->setText(ui->comboBoxRate->currentText());
    serial->setBaudRate(m_chserialrate);
    ui->comboBoxRate->setCurrentIndex(index);
    if (ui->checkBAutoSaveSet->isChecked()) SaveSettings();
}

void MainWindow::Run( void )
{
    if( ui->btnStart->text() == QString("Start"))
    {
        ui->btnStart->setText("Stop");
        enabledpointset(false);
        vplot->CleanPlot();
        openSerialPort();
        Receivedata();
        mSerialTimer.start( mSerialTimeOut );
    }
    else
    {
        mSerialTimer.stop();
        closeSerialPort();
        ui->btnStart->setText("Start");
        MessageBox("Disconnect",Qt::red, Qt::black);
        vplot->MessageLog("Disconnect");
        enabledpointset(true);
    }
}

void MainWindow::Receivedata(void)
{
    QByteArray buf;
    QString data;

    buf = serial->readAll();

    if(!buf.isEmpty())
    {
        data = tr(buf);
        buf.clear();
        vplot->PlotDataUpdate(&data);
        CountBox(QString().setNum(vplot->counts()),Qt::green, Qt::black);
        mRun = true;
    }
}

void MainWindow::on_pushBSave_released()
{
    SaveSettings();
}

void MainWindow::on_pushBScale_released()
{
    vplot->SetRange();
    vplot->mPlot->replot();
    SaveSettings();
}

void MainWindow::on_checkBSaveSet_toggled(bool checked)
{
    if (checked) SaveSettings();
}

void MainWindow::on_lineEditFileDataSave_editingFinished()
{
    if (ui->checkBAutoSaveSet->isChecked()) SaveSettings();
    SMesg->setText(ui->lineEditFileDataSave->displayText());
}

void MainWindow::on_lineEditAxYMin_editingFinished()
{
    if (ui->checkBAutoSaveSet->isChecked()) SaveSettings();
}

void MainWindow::on_lineEditAxYMax_editingFinished()
{
    if (ui->checkBAutoSaveSet->isChecked()) SaveSettings();
}

void MainWindow::on_spinBoxSync_valueChanged(int arg1)
{
    mSerialTimeOut = arg1;
    if(mSerialTimer.isActive())
        mSerialTimer.start( mSerialTimeOut );
}

void MainWindow::on_checkBAutoNameFile_clicked(bool checked)
{
    ui->lineEditFileDataSave->setEnabled(!checked);

    if(checked)
        ui->lineEditFileDataSave->setText("Auto name on \"Start\"");
    else
        ui->lineEditFileDataSave->setText("brga-"+CurrentTime("yyyyMMdd-hhmmss")+".csv");

    ui->lineEditFileDataSave->setEnabled(!checked);

    SMesg->setText(ui->checkBAutoNameFile->text());
}
