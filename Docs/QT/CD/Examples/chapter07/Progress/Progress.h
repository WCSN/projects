/* ======================================================================
**  Progress.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _Progress_h_
#define _Progress_h_

#include <QWidget>

class QProgressBar;

// ======================================================================
class Progress : public QWidget {
    Q_OBJECT
private:
    QProgressBar* m_pprb;
    int           m_nStep;

public:
    Progress(QWidget* pobj = 0);

public slots:
    void slotStep ();
    void slotReset();
};

#endif  //_Progress_h_
