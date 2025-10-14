/***************************************************************************
 **                                                                        **
 **  QCustomPlot, an easy to use, modern plotting widget for Qt            **
 **  Copyright (C) 2011-2018 Emanuel Eichhammer                            **
 **                                                                        **
 **  This program is free software: you can redistribute it and/or modify  **
 **  it under the terms of the GNU General Public License as published by  **
 **  the Free Software Foundation, either version 3 of the License, or     **
 **  (at your option) any later version.                                   **
 **                                                                        **
 **  This program is distributed in the hope that it will be useful,       **
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
 **  GNU General Public License for more details.                          **
 **                                                                        **
 **  You should have received a copy of the GNU General Public License     **
 **  along with this program.  If not, see http://www.gnu.org/licenses/.   **
 **                                                                        **
 ****************************************************************************
 **           Author: Emanuel Eichhammer                                   **
 **  Website/Contact: http://www.qcustomplot.com/                          **
 **             Date: 25.06.18                                             **
 **          Version: 2.0.1                                                **
 ****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtCore>
#include <QtGui>
#include <QSoundEffect>
#include <QGridLayout>
#include <QSerialPort>
#include <QDateTime>
#include <QScreen>

#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class VPlot;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool mRun = false;
    void SaveSettings(void);

private slots:
    void Run(void);
    void on_comboBoxRate_activated(int index=0);
    void on_comboBoxPort_activated(int index=0);
    void Receivedata(void); //serial port
    void on_pushBSave_released();
    void on_pushBScale_released();
    void on_checkBSaveSet_toggled(bool checked);
    void on_lineEditFileDataSave_editingFinished();
    void on_lineEditAxYMin_editingFinished();
    void on_lineEditAxYMax_editingFinished();
    void on_spinBoxSync_valueChanged(int arg1);
    void on_checkBAutoNameFile_clicked(bool checked);

private:
    Ui::MainWindow *ui;

    QTimer mSerialTimer;
    int mSerialTimeOut = 10;

    int m_serialdefrate=9600;
    int m_chserialrate=0;
    QList<QSerialPortInfo> infoserialports;
    QGridLayout *layoutGlob;
    QGridLayout *layoutPL;
    QSerialPort *serial;
    QLabel *SMesg, *SPort, *SRate, *SVers;

    VPlot *vplot;
    QRect pwgt;

    void openSerialPort(void);
    void closeSerialPort(void);
    void enabledpointset(bool);
    void InitSettings(void);

    void MessageBox(const QString message,const QColor &tcolor, const QColor &bcolor);
    void CountBox(const QString message,const QColor &tcolor, const QColor &bcolor);
    void createStatusBar(void);
};

#endif // MAINWINDOW_H
