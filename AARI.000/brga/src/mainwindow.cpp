#include "vplot.h"
#include "options.h"
#include "mainwindow.h"

#include "ui_options.h"
#include "ui_mainwindow.h"

const QString version("0.8.75");

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mainFont(new QFont("SansSerif",8)),
    infoserialports(QSerialPortInfo::availablePorts()),
    serial(new QSerialPort)
{
    mainFont->setStyleStrategy(QFont::PreferAntialias);
    ui->setupUi(this);
    createStatusBar();

    vplot = new VPlot(parent,this);
    opt = new Options(parent,this);

    vplot->initSettings(true);
    vplot->setRange();

    setAutoNameFile(opt->uiopt->checkBAutoNameFile->isChecked());

    ui->pushBScaleY->setEnabled(!opt->uiopt->checkBAutoScaleY->isChecked());
    ui->pushBClean->setEnabled(false);

    connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(Run()));
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::Receivedata);

    createLogs();
}

MainWindow::~MainWindow()
{
    if( serial != nullptr )
        closeSerialPort();

    if (opt->uiopt->checkBAutoSaveSet->isChecked())
        opt->saveSettings();

    opt->close();
    opt->deleteLater();

    closeLogs();
    delete ui;
}

bool MainWindow::isMaxwin(void)
{
    return opt->uiopt->checkBLaunchMax->isChecked();
}

void MainWindow::createStatusBar(void)
{
    QFrame::Shape shape = QFrame::Shape::NoFrame;

    SBarMesg = new QLabel("",this);
    SBarMesg->setAlignment(Qt::AlignLeft);
    SBarMesg->setFrameShape(shape);

    SBarOGL = new QLabel("OpenGL:", this);
    SBarOGL->setAlignment(Qt::AlignCenter);
    SBarOGL->setFrameShape(shape);
    SBarOGL->setMinimumSize(SBarOGL->sizeHint());

    SBarOGLst = new QLabel("OFF", this);
    SBarOGLst->setAlignment(Qt::AlignCenter);
    SBarOGLst->setFrameShape(shape);
    SBarOGLst->setMinimumSize(SBarOGLst->sizeHint());

    SBarPort = new QLabel("", this);
    SBarPort->setAlignment(Qt::AlignCenter);
    SBarPort->setFrameShape(shape);
    SBarPort->setMinimumSize(SBarPort->sizeHint());

    SBarPortSt = new QLabel("Disconnect", this);
    SBarPortSt->setAlignment(Qt::AlignCenter);
    SBarPortSt->setFrameShape(shape);
    SBarPortSt->setMinimumSize(SBarPortSt->sizeHint());

    SBarRate = new QLabel("", this);
    SBarRate->setAlignment(Qt::AlignCenter);
    SBarRate->setFrameShape(shape);
    SBarRate->setMinimumSize(SBarRate->sizeHint());

    SBarVers = new QLabel("v."+version, this);
    SBarVers->setAlignment(Qt::AlignCenter);
    SBarVers->setFrameShape(shape);
    SBarVers->setMinimumSize(SBarVers->sizeHint());

    statusBar()->addWidget(SBarMesg,1);
    statusBar()->addWidget(SBarOGL);
    statusBar()->addWidget(SBarOGLst);
    statusBar()->addWidget(SBarPort);
    statusBar()->addWidget(SBarRate);
    statusBar()->addWidget(SBarPortSt);
    statusBar()->addWidget(SBarVers);
}

void MainWindow::setEnableCnt(bool state)
{
    opt->uiopt->groupBoxSerial->setEnabled(state);
    opt->uiopt->groupBoxPath->setEnabled(state);

    if (vplot->counts() > 0)
        ui->pushBClean->setEnabled(state);
    ui->lineEditFileDataSave->setEnabled((opt->uiopt->checkBAutoNameFile->isChecked()) ? false : state);

    ui->pushBClean->setEnabled(state);
}

void MainWindow::closeSerialPort(void)
{
    serial->clear();
    serial->reset();
    serial->close();

    setEnableCnt(true);
}

void MainWindow::setcomboBoxPort(int index)
{
    if(serial->isOpen()) closeSerialPort();

    SBarPort->setText(opt->uiopt->comboBoxPort->currentText());

    opt->uiopt->comboBoxRate->clear();

    for (auto brate : infoserialports[index].standardBaudRates())
        opt->uiopt->comboBoxRate->addItem(QString::number(brate),(QSerialPort::BaudRate) brate);

    opt->uiopt->comboBoxRate->setCurrentIndex(
        opt->uiopt->comboBoxRate->findText(QString::number(curset.serialBaudRate)));

    if (opt->uiopt->checkBAutoSaveSet->isChecked())
        opt->saveSettings();
}

void MainWindow::setcomboBoxRate(int index)
{
    curset.serialBaudRate = (QSerialPort::BaudRate) opt->uiopt->comboBoxRate->currentData().toInt();
    SBarRate->setText(opt->uiopt->comboBoxRate->itemText(index));

    if (opt->uiopt->checkBAutoSaveSet->isChecked())
        opt->saveSettings();
}

bool MainWindow::openSerialPort(void)
{
    if (serial != nullptr)
        closeSerialPort();

    setParSerialPort();

    vplot->messageLog("Connect: " + serial->portName() + " / " + QString::number(serial->baudRate()));

    if (serial->open(QIODevice::ReadWrite))
    {

        ui->btnStart->setText("Stop");
        vplot->setSavePlotBoxEnabled();
        QString msgsport("Receive data");
        vplot->messageLog(msgsport);
        SBarPortSt->setText(msgsport);
        return true;
    }
    else
    {
        QString msgsport("Disconnect");
        ui->btnStart->setText( "Start" );
        vplot->messageLog(serial->errorString());
        vplot->messageLog(msgsport);
        SBarPortSt->setText(msgsport);
        return false;
    }
}

/*
void MainWindow::Receivedata(void)
{
    static QByteArray buf = serial->readAll();

    loggedBuf(&buf);

    for(int i = 0; i < 2; ++i)
    {
        if ((buf.size() < 37 && buf.size() > 15))
        {
            vplot->setDataPlot(&buf);
            mRun = true;
            buf.clear();
            break;
        }
        else if (buf.size() < 15)
        {
            buf = serial->readAll();
        }
    }
}
*/

void MainWindow::Receivedata(void)
{
    QByteArray buf = serial->readAll();

    loggedBuf(&buf);

    if (!buf.isEmpty() )
    {
        vplot->setDataPlot(&buf);
        mRun = true;
    }
}

void MainWindow::Run(void)
{
    if (ui->btnStart->text() == QString("Start"))
    {
        if ( curset.namedatafile == QString("Auto name on 'Start'") && !opt->uiopt->checkBAutoNameFile->isChecked() )
        {
            ui->lineEditFileDataSave->setFocus();
            vplot->messageLog("Set name for data file!");
            vplot->messageLog("Or set 'Autoname' on Options!");
            return;
        }

        if (openSerialPort())
        {
            vplot->cleanPlot();
            on_pushBScale0_clicked();
            ui->btnStart->setText("Stop");
            //Receivedata();
            mSerialTimer.start(opt->uiopt->spinBoxSync->value());
            setEnableCnt(false);
        }
        else
        {
            setEnableCnt(true);
        }
    }
    else
    {
        mSerialTimer.stop();
        closeSerialPort();
        ui->btnStart->setText("Start");
        vplot->messageLog("Disconnect");
    }
}

void MainWindow::createLogs(void)
{
    if (!QDir(curset.pathlogs).exists())
        if (!QDir().mkpath(curset.pathlogs))
            vplot->messageLog("Path: " + curset.pathlogs + " not created");

    fileBuf = new QFile(curset.pathlogs + "/" + "data_recv.buf");
    fileDmp = new QFile(curset.pathlogs + "/" + "data_recv.dmp");
    fileLog = new QFile(curset.pathlogs + "/" + "data_recv.log");

    if (fileBuf->open(QIODevice::WriteOnly))
    {
        streamBuf = new QDataStream(fileBuf);
        streamBuf->setVersion(QDataStream::Qt_5_14);
        fileBuf->flush();
        vplot->messageLog("Buffer logging create");
    }
    else
        vplot->messageLog("Buffer logging not create!");

    if (fileDmp->open(QIODevice::WriteOnly))
    {
        streamDmp = new QDataStream(fileDmp);
        streamDmp->setVersion(QDataStream::Qt_5_14);
        fileDmp->flush();
        vplot->messageLog("Dmp logging create");
    }
    else
        vplot->messageLog("Dmp logging not create!");

    if (fileLog->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        streamLog = new QTextStream(fileLog);
        streamLog->setCodec("UTF-8");
        fileLog->flush();
        vplot->messageLog("Data logging create");
    }
    else
        vplot->messageLog("Data logging not create!");
}

void MainWindow::loggedBuf(const QByteArray *buf)
{
    if (fileBuf->isOpen())
    {
        *streamBuf << *buf;
        fileBuf->flush();
    }
}

void MainWindow::loggedDmp(const QByteArray *dmp)
{
    if (fileDmp->isOpen())
    {
        *streamDmp << *dmp;
        fileBuf->flush();
    }
}

void MainWindow::loggedDat(const QString *dat)
{
    if (fileLog->isOpen())
    {
        *streamLog << *dat << Qt::endl;
        fileLog->flush();
    }
}

void MainWindow::closeLogs(void)
{
    if (fileLog->isOpen())
        fileLog->close();

    if (fileDmp->isOpen())
        fileDmp->close();

    if (fileBuf->isOpen())
        fileBuf->close();
}

void MainWindow::on_lineEditFileDataSave_editingFinished()
{
    SBarMesg->setText(ui->lineEditFileDataSave->displayText());
    curset.namedatafile = ui->lineEditFileDataSave->text();

    if (opt->uiopt->checkBAutoSaveSet->isChecked())
        opt->saveSettings();
}

void MainWindow::on_lineEditAxYmin_editingFinished()
{
    if (opt->uiopt->checkBAutoSaveSet->isChecked())
        opt->saveSettings();

    ui->pushBScaleY->setEnabled(true);
    vplot->setRangeY(true);
}

void MainWindow::on_lineEditAxYmax_editingFinished()
{
    if (opt->uiopt->checkBAutoSaveSet->isChecked())
        opt->saveSettings();

    ui->pushBScaleY->setEnabled(true);
    vplot->setRangeY(true);
}

void MainWindow::on_lineEditAxX0_editingFinished()
{
    // need save this parameter?
    vplot->setRangeX(true);
}

void MainWindow::on_lineEditAxXsize_editingFinished()
{
    if (opt->uiopt->checkBAutoSaveSet->isChecked())
        on_pushBScaleX_clicked();
    vplot->setRangeX(true);
}

void MainWindow::on_pushBClean_clicked()
{
    if( ui->btnStart->text() == QString("Start"))
    {
        vplot->cleanPlot(false);
        on_pushBScale0_clicked();
    }
}

void MainWindow::on_pushBOptions_clicked()
{
    opt->saveBefore();
    opt->show();
}

void MainWindow::setAutoScale(void)
{
    if(opt->uiopt->checkBAutoSaveSet->isChecked())
        opt->saveSettings();

    vplot->setRangeY();
    ui->pushBScaleY->setEnabled(!opt->uiopt->checkBAutoScaleY->isChecked());
    vplot->mPlot->replot();
}

void MainWindow::setAxixsPlotRange(double setAxisX0, double setAxisX0Size, double setAxYmin, double setAxYmax)
{
    ui->lineEditAxX0->setText(QString().setNum((int)setAxisX0));
    ui->lineEditAxXsize->setText(QString().setNum((int)setAxisX0Size));

    ui->lineEditAxYmin->setText(QString().setNum(setAxYmin,'e',2));
    ui->lineEditAxYmax->setText(QString().setNum(setAxYmax,'e',2));

    vplot->initSettings();
    vplot->setRange();
    vplot->mPlot->replot();

    if (opt->uiopt->checkBAutoSaveSet->isChecked())
        opt->saveSettings();
}

void MainWindow::on_pushBScale0_clicked()
{
    setAxixsPlotRange(defset.AxisX0, defset.AxisX0Size, defset.AxisYmin, defset.AxisYmax);
}

void MainWindow::on_pushBScaleX_clicked()
{
    vplot->setRangeX();
    vplot->mPlot->replot();
}

void MainWindow::on_pushBScaleY_clicked()
{
    vplot->setRangeY();
    vplot->mPlot->replot();
    ui->pushBScaleY->setEnabled(false);
}

void MainWindow::setSyncValue(int arg1)
{
    if(mSerialTimer.isActive())
        mSerialTimer.start(arg1);

    if(opt != nullptr)
        if (opt->uiopt->checkBAutoSaveSet->isChecked())
            opt->saveSettings();
}

void MainWindow::setAutoNameFile(bool checked)
{
    ui->lineEditFileDataSave->setEnabled(!(checked));

    if(checked)
        ui->lineEditFileDataSave->setText("Auto name on 'Start'");
}

void MainWindow::setOpenGLEbable(bool checked)
{
    vplot->setOpenGL(checked);

    if (opt->uiopt->checkBAutoSaveSet->isChecked())
        opt->saveSettings();
}

void MainWindow::setParSerialPort(void)
{
    serial->setPortName(opt->uiopt->comboBoxPort->currentText());
    serial->setBaudRate((QSerialPort::BaudRate) opt->uiopt->comboBoxRate->currentData().toInt());
    serial->setDataBits((QSerialPort::DataBits) opt->uiopt->comboBoxDataBits->currentData().toInt());
    serial->setParity((QSerialPort::Parity) opt->uiopt->comboBoxParity->currentData().toInt());
    serial->setStopBits((QSerialPort::StopBits) opt->uiopt->comboBoxStopBits->currentData().toInt());
    serial->setFlowControl((QSerialPort::FlowControl) opt->uiopt->comboBoxFlowControl->currentData().toInt());

    serial->setTextModeEnabled(true);
    serial->setReadBufferSize(16384);
}
