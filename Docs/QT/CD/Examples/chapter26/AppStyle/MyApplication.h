/* ======================================================================
**  MyApplication.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _MyApplication_h_
#define _MyApplication_h_

#include <QWidget>

// ======================================================================
class MyApplication : public QWidget {
    Q_OBJECT
public:
    MyApplication(QWidget* pwgt = 0);    

public slots:
    void slotChangeStyle(const QString& str);
};

#endif  //_MyApplication_h_
