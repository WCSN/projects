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
#include <QtCore>
#include <QtGui>
#include <QSound>
#include "mainwindow.h"
#include "archive.h"
#include "cockpit.h"
#include "wsettings.h"
#include "clock.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow( QWidget *parent ) :
		QMainWindow(parent), on_offlabel(0)
{
	setupUi(this);

//	QDesktopWidget* pwgt = QApplication::desktop();

//	setGeometry(0, 0, pwgt->width(), pwgt->height());
	setGeometry(100, 100, 1228, 600);

	QFont fntbold;
	fntbold.setBold(true);

	ClockfTBar *clocksm = new ClockfTBar(this);
	clocksm->setFont(fntbold);

	on_offlabel = new QLabel();
	on_offlabel->setFont(fntbold);
	ledonoff();
	connect(net_on_off, SIGNAL(toggled(bool)), this, SLOT(ledonoff()));

	Ui_MainWindow::statusBar->addPermanentWidget(on_offlabel, 0);
	Ui_MainWindow::statusBar->addPermanentWidget(clocksm, 0);

	Ui_MainWindow::dockWidget->setFixedHeight(55);
	Ui_MainWindow::tabWidget->setFixedHeight(500);

	label_3->setNum(horizontalSlider->value());
	label_4->setNum(kNobDip->value());

	cockpit = new Cockpit(tab_cockpit, this);
	archive = new Archive(tab_archive, this);
	wsettings = new WSettings(tab_wsettings, this);

	tabWidget->setCurrentIndex(0);

	setCentralWidget(tabWidget);

	cockpit->SaveDefaultSettings();
	cockpit->RestoreCurrentSettings();

	connect(toolButtonRestL, SIGNAL(released()), cockpit, SLOT(RestoreCurrentSettings()));
	connect(actionRestore_Default_Layout, SIGNAL(activated()), cockpit, SLOT(RestoreDefaultSettings()));
	connect(actionSave_Dafault_Layout, SIGNAL(activated()), cockpit, SLOT(SaveDefaultSettings()));

	connect(btnRunC, SIGNAL(released()), cockpit, SLOT(Run()));
	connect(btnRunA, SIGNAL(released()), archive, SLOT(Run()));

	connect(action_Settings, SIGNAL(activated()), this, SLOT(changeTab()));
	connect(checkBox, SIGNAL(clicked()), wsettings, SLOT(checkBoxSee()));
	connect(kNobDip, SIGNAL(valueChanged(double)), wsettings, SLOT(dialSee()));
	connect(horizontalSlider, SIGNAL(valueChanged(int)), wsettings, SLOT(horslSee()));

	connect(radioBRed, SIGNAL(toggled(bool)), wsettings, SLOT(setcolorled()));
	connect(radioBGreen, SIGNAL(toggled(bool)), wsettings, SLOT(setcolorled()));
	connect(radioBBlue, SIGNAL(toggled(bool)), wsettings, SLOT(setcolorled()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::ledonoff( void )
{
	if( net_on_off->checkState() == Qt::Checked ) on_offlabel->setText(tr("Line: On"));
	else on_offlabel->setText(tr("Line: Off"));
}

void MainWindow::changeTab( void )
{
	tabWidget->setCurrentIndex(2);
}
