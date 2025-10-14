/* ======================================================================
**  LoadMyForm.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/
#ifndef _LoadMyForm_h_
#define _LoadMyForm_h_

#include <QtGui>
#include <QtUiTools>

// ======================================================================
class LoadMyForm : public QWidget {
Q_OBJECT
private:
    QSlider*    m_psld;
    QLCDNumber* m_plcd;

public:
    LoadMyForm(QWidget* pwgt = 0) : QWidget(pwgt)
    {
        QUiLoader* puil = new QUiLoader(this);
        QFile      file(":/MyForm.ui");

        QWidget* pwgtForm = puil->load(&file);
        if (pwgtForm) {
            resize(pwgtForm->size());

            m_psld = pwgtForm->findChild<QSlider*>("m_sld");
            m_plcd = pwgtForm->findChild<QLCDNumber*>("m_lcd");

            QPushButton* pcmdReset = 
                pwgtForm->findChild<QPushButton*>("m_cmdReset");
            connect(pcmdReset, SIGNAL(clicked()), SLOT(slotReset()));

            QPushButton* pcmdQuit = 
                pwgtForm->findChild<QPushButton*>("m_cmdQuit");
            connect(pcmdQuit, SIGNAL(clicked()), qApp, SLOT(quit()));

            //Layout setup
            QHBoxLayout* phbxLayout = new QHBoxLayout;    
            phbxLayout->addWidget(pwgtForm);
            setLayout(phbxLayout);
        }
    }

public slots:
    void slotReset()
    {
        m_psld->setValue(0);
        m_plcd->display(0);
    }
};
#endif  //_LoadMyForm_h_
