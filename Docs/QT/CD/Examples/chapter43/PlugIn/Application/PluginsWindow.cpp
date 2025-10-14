/* ======================================================================
**  PluginsWindow.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <QtGui>
#include "PluginsWindow.h"

// ----------------------------------------------------------------------
PluginsWindow::PluginsWindow(QWidget* pwgt/*=0*/) : QMainWindow(pwgt) 
{
    m_plbl        = new QLabel("this is the test text");
    m_pmnuPlugins = new QMenu("&PluginOperations");

    loadPlugins();
    setCentralWidget(m_plbl);
    menuBar()->addMenu(m_pmnuPlugins);
}

// ----------------------------------------------------------------------
void PluginsWindow::loadPlugins()
{
    QDir dir(QApplication::applicationDirPath());
    if (!dir.cd("plugins")) {
        QMessageBox::critical(0, "", "plugins directory does not exist");
        return;
    }

    foreach (QString strFileName, dir.entryList(QDir::Files)) {
        QPluginLoader loader(dir.absoluteFilePath(strFileName));
        addToMenu(qobject_cast<QObject*>(loader.instance()));
    }
}

// ----------------------------------------------------------------------
void PluginsWindow::addToMenu(QObject* pobj)
{
    if (!pobj) {
        return;
    }
    
    StringInterface* pI = qobject_cast<StringInterface*>(pobj);
    if (pI) {
        QStringList lstOperations = pI->operations();
        foreach (QString str, lstOperations) {
            QAction* pact = new QAction(str, pobj);
            connect(pact, SIGNAL(triggered()), 
                    this, SLOT(slotStringOperation())
                   );
            m_pmnuPlugins->addAction(pact);
        }
    }
}

// ----------------------------------------------------------------------
void PluginsWindow::slotStringOperation()
{
    QAction* pact = qobject_cast<QAction*>(sender());

    StringInterface* pI = qobject_cast<StringInterface*>(pact->parent());

    m_plbl->setText(pI->operation(m_plbl->text(), pact->text()));
}
