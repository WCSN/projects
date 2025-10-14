/********************************************************************************
** Form generated from reading UI file 'plotterbox.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTTERBOX_H
#define UI_PLOTTERBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlotterBox
{
public:
    QGridLayout *gridLayout;
    QSplitter *splMngViewBox;
    QGroupBox *groupPlotBox;
    QPushButton *pushBPrint;
    QCheckBox *checkBoxPDF;
    QCheckBox *checkBoxJPG;
    QPushButton *pushBSavePlot;
    QCheckBox *checkBoxPNG;
    QCheckBox *checkBoxBMP;
    QTextBrowser *mDataLog;
    QTextBrowser *mMsgsLog;

    void setupUi(QWidget *PlotterBox)
    {
        if (PlotterBox->objectName().isEmpty())
            PlotterBox->setObjectName("PlotterBox");
        PlotterBox->setEnabled(true);
        PlotterBox->resize(286, 640);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PlotterBox->sizePolicy().hasHeightForWidth());
        PlotterBox->setSizePolicy(sizePolicy);
        PlotterBox->setMinimumSize(QSize(0, 0));
        PlotterBox->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setPointSize(8);
        PlotterBox->setFont(font);
        gridLayout = new QGridLayout(PlotterBox);
        gridLayout->setSpacing(3);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(3, 3, 3, 3);
        splMngViewBox = new QSplitter(PlotterBox);
        splMngViewBox->setObjectName("splMngViewBox");
        splMngViewBox->setLineWidth(-7);
        splMngViewBox->setOrientation(Qt::Vertical);
        splMngViewBox->setOpaqueResize(true);
        groupPlotBox = new QGroupBox(splMngViewBox);
        groupPlotBox->setObjectName("groupPlotBox");
        groupPlotBox->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupPlotBox->sizePolicy().hasHeightForWidth());
        groupPlotBox->setSizePolicy(sizePolicy1);
        groupPlotBox->setMinimumSize(QSize(270, 40));
        groupPlotBox->setAlignment(Qt::AlignCenter);
        groupPlotBox->setFlat(false);
        pushBPrint = new QPushButton(groupPlotBox);
        pushBPrint->setObjectName("pushBPrint");
        pushBPrint->setGeometry(QRect(204, 6, 58, 26));
        pushBPrint->setMinimumSize(QSize(58, 26));
        pushBPrint->setMaximumSize(QSize(58, 26));
        checkBoxPDF = new QCheckBox(groupPlotBox);
        checkBoxPDF->setObjectName("checkBoxPDF");
        checkBoxPDF->setGeometry(QRect(20, 3, 40, 17));
        checkBoxPDF->setMinimumSize(QSize(40, 0));
        checkBoxPDF->setMaximumSize(QSize(40, 16777215));
        checkBoxPDF->setChecked(true);
        checkBoxJPG = new QCheckBox(groupPlotBox);
        checkBoxJPG->setObjectName("checkBoxJPG");
        checkBoxJPG->setGeometry(QRect(79, 19, 40, 17));
        checkBoxJPG->setMinimumSize(QSize(40, 0));
        checkBoxJPG->setMaximumSize(QSize(40, 16777215));
        pushBSavePlot = new QPushButton(groupPlotBox);
        pushBSavePlot->setObjectName("pushBSavePlot");
        pushBSavePlot->setGeometry(QRect(144, 6, 58, 26));
        pushBSavePlot->setMinimumSize(QSize(58, 26));
        pushBSavePlot->setMaximumSize(QSize(58, 26));
        checkBoxPNG = new QCheckBox(groupPlotBox);
        checkBoxPNG->setObjectName("checkBoxPNG");
        checkBoxPNG->setGeometry(QRect(20, 19, 45, 17));
        checkBoxPNG->setMinimumSize(QSize(45, 17));
        checkBoxPNG->setMaximumSize(QSize(45, 17));
        checkBoxBMP = new QCheckBox(groupPlotBox);
        checkBoxBMP->setObjectName("checkBoxBMP");
        checkBoxBMP->setGeometry(QRect(79, 3, 45, 17));
        checkBoxBMP->setMinimumSize(QSize(45, 17));
        checkBoxBMP->setMaximumSize(QSize(45, 17));
        splMngViewBox->addWidget(groupPlotBox);
        mDataLog = new QTextBrowser(splMngViewBox);
        mDataLog->setObjectName("mDataLog");
        mDataLog->setMinimumSize(QSize(280, 0));
        splMngViewBox->addWidget(mDataLog);
        mMsgsLog = new QTextBrowser(splMngViewBox);
        mMsgsLog->setObjectName("mMsgsLog");
        mMsgsLog->setMinimumSize(QSize(280, 0));
        splMngViewBox->addWidget(mMsgsLog);

        gridLayout->addWidget(splMngViewBox, 0, 0, 1, 1);

        QWidget::setTabOrder(checkBoxPDF, checkBoxPNG);
        QWidget::setTabOrder(checkBoxPNG, checkBoxBMP);
        QWidget::setTabOrder(checkBoxBMP, checkBoxJPG);
        QWidget::setTabOrder(checkBoxJPG, pushBSavePlot);
        QWidget::setTabOrder(pushBSavePlot, pushBPrint);
        QWidget::setTabOrder(pushBPrint, mDataLog);
        QWidget::setTabOrder(mDataLog, mMsgsLog);

        retranslateUi(PlotterBox);

        QMetaObject::connectSlotsByName(PlotterBox);
    } // setupUi

    void retranslateUi(QWidget *PlotterBox)
    {
        PlotterBox->setWindowTitle(QCoreApplication::translate("PlotterBox", "Form", nullptr));
        groupPlotBox->setTitle(QString());
        pushBPrint->setText(QCoreApplication::translate("PlotterBox", "Print", nullptr));
        checkBoxPDF->setText(QCoreApplication::translate("PlotterBox", "pdf", nullptr));
        checkBoxJPG->setText(QCoreApplication::translate("PlotterBox", "jpg", nullptr));
        pushBSavePlot->setText(QCoreApplication::translate("PlotterBox", "Save Plot", nullptr));
        checkBoxPNG->setText(QCoreApplication::translate("PlotterBox", "png", nullptr));
        checkBoxBMP->setText(QCoreApplication::translate("PlotterBox", "bmp", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlotterBox: public Ui_PlotterBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTTERBOX_H
