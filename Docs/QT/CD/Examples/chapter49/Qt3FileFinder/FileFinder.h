/* ======================================================================
**  FileFinder.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2004 by Max Schlee
** ======================================================================
*/

#ifndef _FileFinder_h_
#define _FileFinder_h_

#include <qvbox.h>
#include <qdir.h>

class QLineEdit;
class QTextEdit;

// ======================================================================
class FileFinder : public QWidget {
    Q_OBJECT
private:
    QLineEdit* m_ptxtDir;
    QLineEdit* m_ptxtMask;
    QTextEdit* m_ptxtResult;

public:
    FileFinder(QWidget* pwgt = 0, const char* pszName = 0);    

    void start(QDir dir);

public slots:
    void slotFind  ();
    void slotBrowse();
};

#endif  //_FileFinder_h_
