/********************************************************************************
** Form generated from reading ui file 'MyForm.ui'
**
** Created: Mon 4. Dec 23:19:09 2006
**      by: Qt User Interface Compiler version 4.2.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MYFORM_H
#define UI_MYFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLCDNumber>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_MyForm
{
public:
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QLCDNumber *m_lcd;
    QSlider *m_sld;
    QVBoxLayout *vboxLayout1;
    QPushButton *m_cmdReset;
    QPushButton *m_cmdQuit;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *MyForm)
    {
    MyForm->setObjectName(QString::fromUtf8("MyForm"));
    hboxLayout = new QHBoxLayout(MyForm);
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(9);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    m_lcd = new QLCDNumber(MyForm);
    m_lcd->setObjectName(QString::fromUtf8("m_lcd"));

    vboxLayout->addWidget(m_lcd);

    m_sld = new QSlider(MyForm);
    m_sld->setObjectName(QString::fromUtf8("m_sld"));
    m_sld->setOrientation(Qt::Horizontal);

    vboxLayout->addWidget(m_sld);


    hboxLayout->addLayout(vboxLayout);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    m_cmdReset = new QPushButton(MyForm);
    m_cmdReset->setObjectName(QString::fromUtf8("m_cmdReset"));

    vboxLayout1->addWidget(m_cmdReset);

    m_cmdQuit = new QPushButton(MyForm);
    m_cmdQuit->setObjectName(QString::fromUtf8("m_cmdQuit"));

    vboxLayout1->addWidget(m_cmdQuit);

    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout1->addItem(spacerItem);


    hboxLayout->addLayout(vboxLayout1);


    retranslateUi(MyForm);

    QSize size(400, 300);
    size = size.expandedTo(MyForm->minimumSizeHint());
    MyForm->resize(size);

    QObject::connect(m_sld, SIGNAL(sliderMoved(int)), m_lcd, SLOT(display(int)));
    QObject::connect(m_cmdQuit, SIGNAL(clicked()), MyForm, SLOT(close()));

    QMetaObject::connectSlotsByName(MyForm);
    } // setupUi

    void retranslateUi(QWidget *MyForm)
    {
    MyForm->setWindowTitle(QApplication::translate("MyForm", "DesignedWidget", 0, QApplication::UnicodeUTF8));
    m_cmdReset->setText(QApplication::translate("MyForm", "&Reset", 0, QApplication::UnicodeUTF8));
    m_cmdQuit->setText(QApplication::translate("MyForm", "&Quit", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(MyForm);
    } // retranslateUi

};

namespace Ui {
    class MyForm: public Ui_MyForm {};
} // namespace Ui

#endif // UI_MYFORM_H
