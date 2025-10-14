/* ======================================================================
**  PluginsWindow.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _PluginsWindow_h_
#define _PluginsWindow_h_

#include <QMainWindow>
#include "interfaces.h"

class QLabel;
class QMenu;
// ======================================================================
class PluginsWindow : public QMainWindow {
Q_OBJECT

private:
    QLabel* m_plbl;
    QMenu*  m_pmnuPlugins;

public:
    PluginsWindow(QWidget* pwgt = 0);

    void loadPlugins(             );
    void addToMenu  (QObject* pobj);

protected slots:
    void slotStringOperation();
};
#endif //_PluginsWindow_h_
