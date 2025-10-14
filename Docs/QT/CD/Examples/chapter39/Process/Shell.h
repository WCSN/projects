/* ======================================================================
**  Shell.h --
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _Shell_h_
#define _Shell_h_

#include <QtGui>

// ======================================================================
class Shell : public QWidget {
Q_OBJECT
private:
    QProcess*  m_process;
    QLineEdit* m_ptxtCommand;
    QTextEdit* m_ptxtDisplay;

public:
    // ------------------------------------------------------------------
    Shell(QWidget* pwgt = 0) : QWidget(pwgt)
    {
        m_process     = new QProcess(this);
        m_ptxtDisplay = new QTextEdit;

        QLabel* plbl = new QLabel("&Command:");

        m_ptxtCommand = new QLineEdit("dir");
        plbl->setBuddy(m_ptxtCommand);

        QPushButton* pcmd = new QPushButton("&Enter");

        connect(m_process, 
                SIGNAL(readyReadStandardOutput()), 
                SLOT(slotDataOnStdout())
               );
        connect(m_ptxtCommand, 
                SIGNAL(returnPressed()), 
                SLOT(slotReturnPressed())
               );
        connect(pcmd, SIGNAL(clicked()), SLOT(slotReturnPressed()));

        //Layout setup
        QHBoxLayout* phbxLayout = new QHBoxLayout;    
        phbxLayout->addWidget(plbl);
        phbxLayout->addWidget(m_ptxtCommand);
        phbxLayout->addWidget(pcmd);

        QVBoxLayout* pvbxLayout = new QVBoxLayout;    
        pvbxLayout->addWidget(m_ptxtDisplay);
        pvbxLayout->addLayout(phbxLayout);
        setLayout(pvbxLayout);
    }

public slots:
    // ------------------------------------------------------------------
    void slotDataOnStdout()
    {
        m_ptxtDisplay->append(m_process->readAllStandardOutput());
    }

    // ------------------------------------------------------------------
    void slotReturnPressed()
    {
        QString strCommand = "";
#ifdef Q_WS_WIN
        strCommand = "cmd /C ";
#endif
        strCommand += m_ptxtCommand->text();
        m_process->start(strCommand);
    }
};  
#endif  //_Shell_h_

