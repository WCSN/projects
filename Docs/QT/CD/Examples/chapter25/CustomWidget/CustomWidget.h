/* ======================================================================
**  CustomWidget.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _CustomWidget_h_
#define _CustomWidget_h_

#include <QFrame>

// ======================================================================
class CustomWidget : public QFrame {
    Q_OBJECT
protected:
    int m_nProgress;

    virtual void paintEvent(QPaintEvent*);

public:
    CustomWidget(QWidget* pwgt = 0);

    virtual QSize sizeHint() const;

signals:
    void progressChanged(int);

public slots:
    void slotSetProgress(int n);
};
#endif  //_CustomWidget_h_
