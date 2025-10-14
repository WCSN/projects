/* ======================================================================
**  DocWindow.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _DocWindow_h_
#define _DocWindow_h_

#include <QTextEdit>

// ======================================================================
class DocWindow: public QTextEdit {
Q_OBJECT
private:
    QString m_strFileName;

public:
    DocWindow(QWidget* pwgt = 0);

signals:
    void changeWindowTitle(const QString&);

public slots:
    void slotLoad  ();
    void slotSave  ();
    void slotSaveAs();
};
#endif //_DocWindow_h_
