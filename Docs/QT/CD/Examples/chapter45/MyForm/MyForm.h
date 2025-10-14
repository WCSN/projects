/* ======================================================================
**  MyForm.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _MyForm_h_
#define _MyForm_h_

#include "ui_MyForm.h"

// ======================================================================
class MyForm : public QWidget, public Ui::MyForm {
    Q_OBJECT

public:
    MyForm(QWidget* pwgt = 0) : QWidget(pwgt)
    {
        setupUi(this);

        connect(m_cmdReset, SIGNAL(clicked()), SLOT(slotReset()));
    }

public slots:
    void slotReset()
    {
        m_sld->setValue(0);
        m_lcd->display(0);
    }
};
#endif  //_MyForm_h_
