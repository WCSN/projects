/* ======================================================================
**  MDIProgram.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/
#ifndef _MDIProgram_h_
#define _MDIProgram_h_

#include <QMainWindow>

class QMenu;
class QMdiArea;
class QSignalMapper;
class DocWindow;

// ======================================================================
class MDIProgram : public QMainWindow {
    Q_OBJECT
private:
    QMdiArea*      m_pma;
    QMenu*         m_pmnuWindows;
    QSignalMapper* m_psigMapper;

    DocWindow* MDIProgram::createNewDoc();

public:
    MDIProgram(QWidget* pwgt = 0);

private slots:
    void slotChangeWindowTitle(const QString&);

private slots:
    void slotNewDoc            (        );
    void slotLoad              (        );
    void slotSave              (        );
    void slotSaveAs            (        );
    void slotAbout             (        );
    void slotWindows           (        );
    void slotSetActiveSubWindow(QWidget*);
};
#endif //_MDIProgram_h_
