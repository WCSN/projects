/* ======================================================================
**  Installer.cpp --
** ======================================================================
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include "Installer.h"

// ----------------------------------------------------------------------
Installer::Installer(QWidget* pwgt/*=0*/) : QWidget(pwgt)
{
    m_process    = new QProcess(this);
    m_psigMapper = new QSignalMapper(this);

    connect(m_psigMapper, SIGNAL(mapped(const QString&)), 
            this,         SLOT(slotExecute(const QString&))
           );

    QPushButton* pcmdExamples = 
        new QPushButton(tr("Install Book &Examples"));
    connect(pcmdExamples, SIGNAL(clicked()), 
            m_psigMapper, SLOT(map())
           );
    m_psigMapper->setMapping(pcmdExamples, "Autorun/Examples.exe");

    QPushButton* pcmdSDK = 
        new QPushButton(tr("Install &Qt SDK"));
    connect(pcmdSDK, SIGNAL(clicked()), m_psigMapper, SLOT(map()));
     m_psigMapper->setMapping(pcmdSDK, 
                              "Install/Windows/qt-sdk-win-opensource-2009.03.1.exe"
                             );

    QPushButton* pcmdBrowseCD = new QPushButton(tr("&Browse DVD"));
    connect(pcmdBrowseCD, SIGNAL(clicked()), 
            m_psigMapper, SLOT(map())
           );
    m_psigMapper->setMapping(pcmdBrowseCD, "explorer .\\");

    QPushButton* pcmdWeb = new QPushButton(tr("&Web Support"));
    connect(pcmdWeb, SIGNAL(clicked()), m_psigMapper, SLOT(map()));
    m_psigMapper->setMapping(pcmdWeb, 
                             "explorer "
                             "http://www.geocities.com/mslerm/"
                            );

    QPushButton* pcmdQuit = new QPushButton(tr("C&lose"));
    connect(pcmdQuit, SIGNAL(clicked()), qApp, SLOT(quit()));

    QLabel* plblCover = new QLabel;
    plblCover->setPixmap(QPixmap(":/resources/cover.png"));
    //plblCover->setFrameStyle(QFrame::Box | QFrame::Raised);
    //plblCover->setLineWidth(3);

    //Layout setup
    QVBoxLayout* pvboxLayout = new QVBoxLayout;
    pvboxLayout->addWidget(pcmdExamples);
    pvboxLayout->addWidget(pcmdSDK);
    pvboxLayout->addWidget(pcmdBrowseCD);
    pvboxLayout->addWidget(pcmdWeb);
    pvboxLayout->addStretch(1);
    pvboxLayout->addWidget(pcmdQuit);

    QHBoxLayout* phboxLayout = new QHBoxLayout;    
    phboxLayout->addWidget(plblCover);        
    phboxLayout->addLayout(pvboxLayout);        
    setLayout(phboxLayout);
}

// ----------------------------------------------------------------------
void Installer::slotExecute(const QString& str)
{
    m_process->start(str);
}
