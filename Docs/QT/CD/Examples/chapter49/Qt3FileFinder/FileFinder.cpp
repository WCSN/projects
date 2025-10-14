/* ======================================================================
**  FileFinder.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2004 by Max Schlee
** ======================================================================
*/  
#include <qapplication.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qfiledialog.h>
#include "fileopen.xpm"
#include "FileFinder.h"

// ----------------------------------------------------------------------
FileFinder::FileFinder(QWidget* pwgt/*= 0*/, const char* pszName/*= 0*/)
    : QWidget(pwgt, pszName) 
{
    m_ptxtDir    = new QLineEdit(QDir::current().absPath(), this);
    m_ptxtMask   = new QLineEdit("*.cpp *.h", this);
    m_ptxtResult = new QTextEdit(this);

    QLabel*      plblDir  = new QLabel("&Directory", this);
    QLabel*      plblMask = new QLabel("&Mask", this);
    QPushButton* pcmdDir  = new QPushButton(QPixmap(fileopen), "", this);
    QPushButton* pcmdFind = new QPushButton("&Find", this);

    connect(pcmdDir, SIGNAL(clicked()), SLOT(slotBrowse()));
    connect(pcmdFind, SIGNAL(clicked()), SLOT(slotFind()));

    plblDir->setBuddy(m_ptxtDir);
    plblMask->setBuddy(m_ptxtMask);

    QGridLayout* ptopLayout = new QGridLayout(this, 3, 3, 5, 5);
    ptopLayout->addWidget(plblDir, 0, 0);
    ptopLayout->addWidget(plblMask, 1, 0);
    ptopLayout->addWidget(m_ptxtDir, 0, 1);
    ptopLayout->addWidget(m_ptxtMask, 1, 1);
    ptopLayout->addWidget(pcmdDir, 0, 2);
    ptopLayout->addWidget(pcmdFind, 1, 2);
    ptopLayout->addMultiCellWidget(m_ptxtResult, 2, 2, 0, 2);
}

// ----------------------------------------------------------------------
void FileFinder::slotBrowse()
{
    QString str = QFileDialog::getExistingDirectory(m_ptxtDir->text());
    m_ptxtDir->setText(str);
}

// ----------------------------------------------------------------------
void FileFinder::slotFind()
{
    start(QDir(m_ptxtDir->text()));
}

// ----------------------------------------------------------------------
void FileFinder::start(QDir dir)
{
    qApp->processEvents();

    QStringList::Iterator it;
    QStringList listFiles = 
        dir.entryList(m_ptxtMask->text(), QDir::Files);

    for (it = listFiles.begin(); it != listFiles.end(); ++it) {
        m_ptxtResult->append(dir.absFilePath(*it));
    }

    QStringList listDir = dir.entryList(QDir::Dirs);
    for (it = listDir.begin(); it != listDir.end(); ++it) {
        if (*it == "." || *it == "..") {
            continue;
        }
        start(QDir(dir.absFilePath(*it)));
    }
}
