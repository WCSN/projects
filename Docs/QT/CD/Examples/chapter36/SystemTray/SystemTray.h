/* ======================================================================
**  SystemTray.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#ifndef _SystemTray_h_
#define _SystemTray_h_

#include <QLabel>

class QSystemTrayIcon;
class QMenu;

// ======================================================================
class SystemTray : public QLabel {
Q_OBJECT
private:
    QSystemTrayIcon* m_ptrayIcon;
    QMenu*           m_ptrayIconMenu;
    bool             m_bIconSwitcher;

protected:
    virtual void closeEvent(QCloseEvent*);

public:
    SystemTray(QWidget* pwgt = 0);

public slots:
    void slotShowHide   ();
    void slotShowMessage();
    void slotChangeIcon ();
};
#endif  //_SystemTray_h_
