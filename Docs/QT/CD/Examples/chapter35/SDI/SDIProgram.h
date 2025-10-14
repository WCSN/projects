/* ======================================================================
**  SDIProgram.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _SDIProgram_h_
#define _SDIProgram_h_

#include <QtGui>
#include "DocWindow.h"
#include "SDIProgram.h"

// ======================================================================
class SDIProgram : public QMainWindow {
Q_OBJECT
public:
    SDIProgram(QWidget* pwgt = 0) : QMainWindow(pwgt) 
    {
        QMenu*     pmnuFile = new QMenu("&File");
        QMenu*     pmnuHelp = new QMenu("&Help");
        DocWindow* pdoc     = new DocWindow;

        pmnuFile->addAction("&Open...", 
                            pdoc, 
                            SLOT(slotLoad()), 
                            QKeySequence("CTRL+O")
                           );
        pmnuFile->addAction("&Save", 
                            pdoc, 
                            SLOT(slotSave()), 
                            QKeySequence("CTRL+S")
                           );
        pmnuFile->addAction("S&ave As...", 
                            pdoc, 
                            SLOT(slotSaveAs())
                           );
        pmnuFile->addSeparator();
        pmnuFile->addAction("&Quit", 
                            qApp, 
                            SLOT(quit()), 
                            QKeySequence("CTRL+Q")
                           );
        pmnuHelp->addAction("&About", 
                            this, 
                            SLOT(slotAbout()), 
                            Qt::Key_F1
                           );

        menuBar()->addMenu(pmnuFile);
        menuBar()->addMenu(pmnuHelp);

        setCentralWidget(pdoc);
        connect(pdoc, 
                SIGNAL(changeWindowTitle(const QString&)), 
                SLOT(slotChangeWindowTitle(const QString&))
               );

        statusBar()->showMessage("Ready", 2000);
    }

public slots:
    void slotAbout()
    {
        QMessageBox::about(this, "Application", "SDI Example");
    }

    void slotChangeWindowTitle(const QString& str)
    {
        setWindowTitle(str);
    }
};
#endif  //_SDIProgram_h_
