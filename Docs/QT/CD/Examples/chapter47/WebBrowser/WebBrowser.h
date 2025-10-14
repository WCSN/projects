/* ======================================================================
**  WebBrowser.h - 
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#ifndef _WebBrowser_h_
#define _WebBrowser_h_

#include <QWidget>

class QLineEdit;
class QWebView;
class QPushButton;

// =============================================================
class WebBrowser : public QWidget{
Q_OBJECT
private:
    QLineEdit*    m_ptxt;
    QWebView*     m_pwv;
    QPushButton*  m_pcmdBack;
    QPushButton*  m_pcmdForward;

public:
    WebBrowser(QWidget* wgt = 0);

private slots:
    void slotGo      (    );
    void slotFinished(bool);
};
#endif //_WebBrowser_h_

