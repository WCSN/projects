/* ======================================================================
**  FileFinder.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _FileFinder_h_
#define _FileFinder_h_

#include <QWidget>
#include <QDir>

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
    FileFinder(QWidget* pwgt = 0);    

    void start(const QDir& dir);

public slots:
    void slotFind  ();
    void slotBrowse();
};

#endif  //_FileFinder_h_
