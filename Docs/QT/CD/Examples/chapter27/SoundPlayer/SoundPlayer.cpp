/* ======================================================================
**  SoundPlayer.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include "SoundPlayer.h"

// ----------------------------------------------------------------------
SoundPlayer::SoundPlayer(QWidget* pwgt/*=0*/) : QWidget(pwgt)
{
    if (!QSound::isAvailable()) {
        QMessageBox::warning(this, 
                             "No Sound",
                             "Sorry, I can't play the sound"
                            );
    }                                                           
    QPushButton* pcmdPlay = new QPushButton("&Play");
    QPushButton* pcmdStop = new QPushButton("&Stop");
    QPushButton* pcmdLoop = new QPushButton("&Loop");

    pcmdLoop->setCheckable(true);
    pcmdLoop->setChecked(true);

    connect(pcmdPlay, SIGNAL(clicked()), SLOT(slotPlay()));
    connect(pcmdStop, SIGNAL(clicked()), SLOT(slotStop()));
    connect(pcmdLoop, SIGNAL(toggled(bool)), SLOT(slotLoopToggled(bool)));

    m_psnd = new QSound("music/yesterday.wav", this);

    //Layout setup
    QHBoxLayout* phbxLayout = new QHBoxLayout;    
    phbxLayout->addWidget(pcmdPlay);
    phbxLayout->addWidget(pcmdStop);
    phbxLayout->addWidget(pcmdLoop);
    setLayout(phbxLayout);    

    slotLoopToggled(true);
}

// ----------------------------------------------------------------------
void SoundPlayer::slotPlay()
{
    m_psnd->play();
}

// ----------------------------------------------------------------------
void SoundPlayer::slotStop()
{
    m_psnd->stop();
}

// ----------------------------------------------------------------------
void SoundPlayer::slotLoopToggled(bool b)
{
    m_psnd->setLoops(b ? -1 : 1);
    m_psnd->stop();
}
