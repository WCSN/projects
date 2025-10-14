/********************************************************************************
** Form generated from reading UI file 'options.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONS_H
#define UI_OPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Options
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBoxPath;
    QPushButton *pushBPathLogs;
    QLineEdit *lineEditPathLogs;
    QLineEdit *lineEditPathData;
    QLabel *label_data;
    QLabel *label_logs;
    QPushButton *pushBPathData;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushBSetAllDef;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushBOk;
    QPushButton *pushBCancel;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayoutOpt;
    QGroupBox *groupBoxSerial;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QComboBox *comboBoxPort;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QComboBox *comboBoxRate;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *comboBoxDataBits;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QComboBox *comboBoxParity;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QComboBox *comboBoxStopBits;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QComboBox *comboBoxFlowControl;
    QHBoxLayout *horizontalLayout;
    QLabel *labelSpin;
    QSpinBox *spinBoxSync;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushBSerialPortDefSet;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBoxPlot;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBAutoSaveSet;
    QCheckBox *checkBLaunchMax;
    QFrame *line;
    QCheckBox *checkBAutoNameFile;
    QCheckBox *checkBAutoScaleY;
    QFrame *line_2;
    QCheckBox *checkBOGL;
    QTextBrowser *textBrowserNoteReset;
    QGroupBox *groupBox;
    QLineEdit *lineEditAxYmin;
    QLineEdit *lineEditAxX0;
    QLineEdit *lineEditAxXsize;
    QLineEdit *lineEditAxYmax;
    QLabel *labelXsize;
    QLabel *labelYscale;
    QPushButton *pushBPlotDefSave;
    QPushButton *pushBPlotDefReset;
    QLabel *labelX0;
    QLabel *labelMax;

    void setupUi(QDialog *Options)
    {
        if (Options->objectName().isEmpty())
            Options->setObjectName("Options");
        Options->setWindowModality(Qt::NonModal);
        Options->resize(837, 410);
        Options->setWindowTitle(QString::fromUtf8("BrGA Options"));
        Options->setModal(true);
        gridLayout = new QGridLayout(Options);
        gridLayout->setSpacing(3);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(3, 3, 3, 3);
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        groupBoxPath = new QGroupBox(Options);
        groupBoxPath->setObjectName("groupBoxPath");
        groupBoxPath->setMinimumSize(QSize(780, 72));
        groupBoxPath->setMaximumSize(QSize(750, 72));
        groupBoxPath->setFlat(true);
        pushBPathLogs = new QPushButton(groupBoxPath);
        pushBPathLogs->setObjectName("pushBPathLogs");
        pushBPathLogs->setEnabled(true);
        pushBPathLogs->setGeometry(QRect(703, 45, 60, 25));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushBPathLogs->sizePolicy().hasHeightForWidth());
        pushBPathLogs->setSizePolicy(sizePolicy);
        pushBPathLogs->setMinimumSize(QSize(60, 25));
        pushBPathLogs->setMaximumSize(QSize(60, 25));
        QFont font;
        font.setPointSize(8);
        pushBPathLogs->setFont(font);
        pushBPathLogs->setLayoutDirection(Qt::LeftToRight);
        pushBPathLogs->setCheckable(false);
        pushBPathLogs->setFlat(false);
        lineEditPathLogs = new QLineEdit(groupBoxPath);
        lineEditPathLogs->setObjectName("lineEditPathLogs");
        lineEditPathLogs->setGeometry(QRect(83, 46, 615, 23));
        lineEditPathLogs->setMinimumSize(QSize(615, 23));
        lineEditPathLogs->setMaximumSize(QSize(615, 23));
        lineEditPathData = new QLineEdit(groupBoxPath);
        lineEditPathData->setObjectName("lineEditPathData");
        lineEditPathData->setGeometry(QRect(83, 21, 615, 23));
        lineEditPathData->setMinimumSize(QSize(615, 23));
        lineEditPathData->setMaximumSize(QSize(615, 23));
        label_data = new QLabel(groupBoxPath);
        label_data->setObjectName("label_data");
        label_data->setGeometry(QRect(-1, 25, 81, 20));
        label_data->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_logs = new QLabel(groupBoxPath);
        label_logs->setObjectName("label_logs");
        label_logs->setGeometry(QRect(-1, 44, 81, 23));
        label_logs->setMinimumSize(QSize(0, 23));
        label_logs->setMaximumSize(QSize(16777215, 23));
        label_logs->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushBPathData = new QPushButton(groupBoxPath);
        pushBPathData->setObjectName("pushBPathData");
        pushBPathData->setEnabled(true);
        pushBPathData->setGeometry(QRect(703, 20, 60, 25));
        sizePolicy.setHeightForWidth(pushBPathData->sizePolicy().hasHeightForWidth());
        pushBPathData->setSizePolicy(sizePolicy);
        pushBPathData->setMinimumSize(QSize(60, 25));
        pushBPathData->setMaximumSize(QSize(60, 25));
        pushBPathData->setFont(font);
        pushBPathData->setLayoutDirection(Qt::LeftToRight);
        pushBPathData->setCheckable(false);
        pushBPathData->setFlat(false);

        gridLayout_3->addWidget(groupBoxPath, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(Options);
        groupBox_2->setObjectName("groupBox_2");
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        groupBox_2->setMinimumSize(QSize(750, 32));
        groupBox_2->setMaximumSize(QSize(750, 32));
        groupBox_2->setFlat(true);
        layoutWidget = new QWidget(groupBox_2);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(5, 4, 746, 26));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget);
        horizontalLayout_8->setSpacing(3);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 15, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        pushBSetAllDef = new QPushButton(layoutWidget);
        pushBSetAllDef->setObjectName("pushBSetAllDef");
        pushBSetAllDef->setMinimumSize(QSize(110, 24));
        pushBSetAllDef->setMaximumSize(QSize(100, 24));
        QFont font1;
        font1.setBold(false);
        pushBSetAllDef->setFont(font1);

        horizontalLayout_8->addWidget(pushBSetAllDef);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);

        pushBOk = new QPushButton(layoutWidget);
        pushBOk->setObjectName("pushBOk");
        pushBOk->setMinimumSize(QSize(70, 24));
        pushBOk->setMaximumSize(QSize(70, 24));
        QFont font2;
        font2.setBold(true);
        pushBOk->setFont(font2);
        pushBOk->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout_8->addWidget(pushBOk);

        pushBCancel = new QPushButton(layoutWidget);
        pushBCancel->setObjectName("pushBCancel");
        sizePolicy.setHeightForWidth(pushBCancel->sizePolicy().hasHeightForWidth());
        pushBCancel->setSizePolicy(sizePolicy);
        pushBCancel->setMinimumSize(QSize(70, 24));
        pushBCancel->setMaximumSize(QSize(70, 24));
        pushBCancel->setFont(font2);
        pushBCancel->setLayoutDirection(Qt::LeftToRight);
        pushBCancel->setAutoFillBackground(false);
        pushBCancel->setAutoDefault(false);

        horizontalLayout_8->addWidget(pushBCancel);

        horizontalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_4);


        gridLayout_3->addWidget(groupBox_2, 3, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 2, 0, 1, 1);


        gridLayout->addLayout(gridLayout_3, 4, 0, 1, 1);

        gridLayoutOpt = new QGridLayout();
        gridLayoutOpt->setObjectName("gridLayoutOpt");
        groupBoxSerial = new QGroupBox(Options);
        groupBoxSerial->setObjectName("groupBoxSerial");
        groupBoxSerial->setMinimumSize(QSize(200, 250));
        groupBoxSerial->setMaximumSize(QSize(200, 250));
        groupBoxSerial->setFlat(false);
        layoutWidget1 = new QWidget(groupBoxSerial);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(5, 17, 191, 224));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName("label_4");
        label_4->setMinimumSize(QSize(75, 23));
        label_4->setMaximumSize(QSize(75, 23));

        horizontalLayout_2->addWidget(label_4);

        comboBoxPort = new QComboBox(layoutWidget1);
        comboBoxPort->setObjectName("comboBoxPort");
        sizePolicy.setHeightForWidth(comboBoxPort->sizePolicy().hasHeightForWidth());
        comboBoxPort->setSizePolicy(sizePolicy);
        comboBoxPort->setMinimumSize(QSize(95, 23));
        comboBoxPort->setMaximumSize(QSize(95, 23));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Liberation Mono")});
        font3.setPointSize(8);
        font3.setBold(false);
        comboBoxPort->setFont(font3);

        horizontalLayout_2->addWidget(comboBoxPort);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label = new QLabel(layoutWidget1);
        label->setObjectName("label");
        label->setMinimumSize(QSize(75, 23));
        label->setMaximumSize(QSize(75, 23));

        horizontalLayout_5->addWidget(label);

        comboBoxRate = new QComboBox(layoutWidget1);
        comboBoxRate->setObjectName("comboBoxRate");
        sizePolicy.setHeightForWidth(comboBoxRate->sizePolicy().hasHeightForWidth());
        comboBoxRate->setSizePolicy(sizePolicy);
        comboBoxRate->setMinimumSize(QSize(95, 23));
        comboBoxRate->setMaximumSize(QSize(95, 23));
        comboBoxRate->setFont(font3);

        horizontalLayout_5->addWidget(comboBoxRate);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(75, 23));
        label_2->setMaximumSize(QSize(75, 23));

        horizontalLayout_3->addWidget(label_2);

        comboBoxDataBits = new QComboBox(layoutWidget1);
        comboBoxDataBits->setObjectName("comboBoxDataBits");
        comboBoxDataBits->setMinimumSize(QSize(95, 24));
        comboBoxDataBits->setMaximumSize(QSize(95, 24));
        comboBoxDataBits->setFont(font);

        horizontalLayout_3->addWidget(comboBoxDataBits);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName("label_3");
        label_3->setMinimumSize(QSize(75, 23));
        label_3->setMaximumSize(QSize(75, 23));

        horizontalLayout_4->addWidget(label_3);

        comboBoxParity = new QComboBox(layoutWidget1);
        comboBoxParity->setObjectName("comboBoxParity");
        comboBoxParity->setMinimumSize(QSize(95, 23));
        comboBoxParity->setMaximumSize(QSize(95, 23));
        comboBoxParity->setFont(font);

        horizontalLayout_4->addWidget(comboBoxParity);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName("label_5");
        label_5->setMinimumSize(QSize(75, 23));
        label_5->setMaximumSize(QSize(75, 23));

        horizontalLayout_6->addWidget(label_5);

        comboBoxStopBits = new QComboBox(layoutWidget1);
        comboBoxStopBits->setObjectName("comboBoxStopBits");
        comboBoxStopBits->setMinimumSize(QSize(95, 24));
        comboBoxStopBits->setMaximumSize(QSize(95, 24));
        comboBoxStopBits->setFont(font);

        horizontalLayout_6->addWidget(comboBoxStopBits);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName("label_6");
        label_6->setMinimumSize(QSize(78, 23));
        label_6->setMaximumSize(QSize(75, 23));

        horizontalLayout_7->addWidget(label_6);

        comboBoxFlowControl = new QComboBox(layoutWidget1);
        comboBoxFlowControl->setObjectName("comboBoxFlowControl");
        comboBoxFlowControl->setMinimumSize(QSize(95, 23));
        comboBoxFlowControl->setMaximumSize(QSize(95, 23));
        comboBoxFlowControl->setFont(font);

        horizontalLayout_7->addWidget(comboBoxFlowControl);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        labelSpin = new QLabel(layoutWidget1);
        labelSpin->setObjectName("labelSpin");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelSpin->sizePolicy().hasHeightForWidth());
        labelSpin->setSizePolicy(sizePolicy1);
        labelSpin->setMinimumSize(QSize(75, 23));
        labelSpin->setMaximumSize(QSize(75, 23));
        labelSpin->setLayoutDirection(Qt::RightToLeft);
        labelSpin->setTextFormat(Qt::AutoText);
        labelSpin->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(labelSpin);

        spinBoxSync = new QSpinBox(layoutWidget1);
        spinBoxSync->setObjectName("spinBoxSync");
        sizePolicy.setHeightForWidth(spinBoxSync->sizePolicy().hasHeightForWidth());
        spinBoxSync->setSizePolicy(sizePolicy);
        spinBoxSync->setMinimumSize(QSize(95, 23));
        spinBoxSync->setMaximumSize(QSize(95, 23));
        spinBoxSync->setFont(font3);
        spinBoxSync->setToolTipDuration(7);
        spinBoxSync->setWrapping(false);
        spinBoxSync->setFrame(true);
        spinBoxSync->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBoxSync->setMinimum(1);
        spinBoxSync->setMaximum(10000);
        spinBoxSync->setSingleStep(1);
        spinBoxSync->setValue(10000);

        horizontalLayout->addWidget(spinBoxSync);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(0);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_2);

        pushBSerialPortDefSet = new QPushButton(layoutWidget1);
        pushBSerialPortDefSet->setObjectName("pushBSerialPortDefSet");
        sizePolicy.setHeightForWidth(pushBSerialPortDefSet->sizePolicy().hasHeightForWidth());
        pushBSerialPortDefSet->setSizePolicy(sizePolicy);
        pushBSerialPortDefSet->setMinimumSize(QSize(32, 25));
        pushBSerialPortDefSet->setMaximumSize(QSize(32, 25));
        pushBSerialPortDefSet->setFont(font);
        pushBSerialPortDefSet->setLayoutDirection(Qt::LeftToRight);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/btnS0.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushBSerialPortDefSet->setIcon(icon);
        pushBSerialPortDefSet->setCheckable(false);
        pushBSerialPortDefSet->setFlat(false);

        horizontalLayout_9->addWidget(pushBSerialPortDefSet);

        horizontalSpacer_5 = new QSpacerItem(6, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_9);


        gridLayoutOpt->addWidget(groupBoxSerial, 0, 1, 1, 1);

        groupBoxPlot = new QGroupBox(Options);
        groupBoxPlot->setObjectName("groupBoxPlot");
        groupBoxPlot->setMinimumSize(QSize(580, 250));
        groupBoxPlot->setMaximumSize(QSize(550, 250));
        groupBoxPlot->setLayoutDirection(Qt::LeftToRight);
        groupBoxPlot->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBoxPlot->setFlat(true);
        groupBoxPlot->setCheckable(false);
        layoutWidget2 = new QWidget(groupBoxPlot);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(8, 20, 191, 156));
        verticalLayout = new QVBoxLayout(layoutWidget2);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        checkBAutoSaveSet = new QCheckBox(layoutWidget2);
        checkBAutoSaveSet->setObjectName("checkBAutoSaveSet");
        sizePolicy1.setHeightForWidth(checkBAutoSaveSet->sizePolicy().hasHeightForWidth());
        checkBAutoSaveSet->setSizePolicy(sizePolicy1);
        checkBAutoSaveSet->setMinimumSize(QSize(18, 21));
        checkBAutoSaveSet->setMaximumSize(QSize(16777215, 16777215));
        checkBAutoSaveSet->setLayoutDirection(Qt::LeftToRight);
        checkBAutoSaveSet->setChecked(true);

        verticalLayout->addWidget(checkBAutoSaveSet);

        checkBLaunchMax = new QCheckBox(layoutWidget2);
        checkBLaunchMax->setObjectName("checkBLaunchMax");
        checkBLaunchMax->setEnabled(true);
        checkBLaunchMax->setLayoutDirection(Qt::LeftToRight);
        checkBLaunchMax->setChecked(true);

        verticalLayout->addWidget(checkBLaunchMax);

        line = new QFrame(layoutWidget2);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        verticalLayout->addWidget(line);

        checkBAutoNameFile = new QCheckBox(layoutWidget2);
        checkBAutoNameFile->setObjectName("checkBAutoNameFile");
        sizePolicy1.setHeightForWidth(checkBAutoNameFile->sizePolicy().hasHeightForWidth());
        checkBAutoNameFile->setSizePolicy(sizePolicy1);
        checkBAutoNameFile->setMinimumSize(QSize(120, 21));
        checkBAutoNameFile->setMaximumSize(QSize(16777215, 16777215));
        checkBAutoNameFile->setLayoutDirection(Qt::LeftToRight);
        checkBAutoNameFile->setChecked(true);

        verticalLayout->addWidget(checkBAutoNameFile);

        checkBAutoScaleY = new QCheckBox(layoutWidget2);
        checkBAutoScaleY->setObjectName("checkBAutoScaleY");
        sizePolicy1.setHeightForWidth(checkBAutoScaleY->sizePolicy().hasHeightForWidth());
        checkBAutoScaleY->setSizePolicy(sizePolicy1);
        checkBAutoScaleY->setMinimumSize(QSize(18, 21));
        checkBAutoScaleY->setMaximumSize(QSize(16777215, 16777215));
        checkBAutoScaleY->setLayoutDirection(Qt::LeftToRight);
        checkBAutoScaleY->setChecked(true);

        verticalLayout->addWidget(checkBAutoScaleY);

        line_2 = new QFrame(layoutWidget2);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::Shape::HLine);
        line_2->setFrameShadow(QFrame::Shadow::Sunken);

        verticalLayout->addWidget(line_2);

        checkBOGL = new QCheckBox(layoutWidget2);
        checkBOGL->setObjectName("checkBOGL");
        checkBOGL->setEnabled(true);
        checkBOGL->setLayoutDirection(Qt::LeftToRight);
        checkBOGL->setChecked(false);

        verticalLayout->addWidget(checkBOGL);

        textBrowserNoteReset = new QTextBrowser(groupBoxPlot);
        textBrowserNoteReset->setObjectName("textBrowserNoteReset");
        textBrowserNoteReset->setEnabled(true);
        textBrowserNoteReset->setGeometry(QRect(200, 14, 380, 184));
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy2.setHorizontalStretch(192);
        sizePolicy2.setVerticalStretch(120);
        sizePolicy2.setHeightForWidth(textBrowserNoteReset->sizePolicy().hasHeightForWidth());
        textBrowserNoteReset->setSizePolicy(sizePolicy2);
        textBrowserNoteReset->setMinimumSize(QSize(380, 184));
        textBrowserNoteReset->setMaximumSize(QSize(380, 140));
        QPalette palette;
        QBrush brush(QColor(240, 240, 240, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush);
        QBrush brush1(QColor(232, 231, 223, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        textBrowserNoteReset->setPalette(palette);
        textBrowserNoteReset->setFont(font);
        textBrowserNoteReset->setFrameShape(QFrame::Box);
        textBrowserNoteReset->setFrameShadow(QFrame::Sunken);
        textBrowserNoteReset->setLineWidth(1);
        groupBox = new QGroupBox(groupBoxPlot);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(0, 194, 551, 51));
        groupBox->setFlat(true);
        lineEditAxYmin = new QLineEdit(groupBox);
        lineEditAxYmin->setObjectName("lineEditAxYmin");
        lineEditAxYmin->setEnabled(true);
        lineEditAxYmin->setGeometry(QRect(256, 19, 78, 25));
        sizePolicy.setHeightForWidth(lineEditAxYmin->sizePolicy().hasHeightForWidth());
        lineEditAxYmin->setSizePolicy(sizePolicy);
        lineEditAxYmin->setMinimumSize(QSize(25, 25));
        lineEditAxYmin->setMaximumSize(QSize(78, 25));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Liberation Mono")});
        font4.setPointSize(8);
        lineEditAxYmin->setFont(font4);
        lineEditAxYmin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditAxX0 = new QLineEdit(groupBox);
        lineEditAxX0->setObjectName("lineEditAxX0");
        lineEditAxX0->setEnabled(true);
        lineEditAxX0->setGeometry(QRect(27, 19, 78, 25));
        sizePolicy.setHeightForWidth(lineEditAxX0->sizePolicy().hasHeightForWidth());
        lineEditAxX0->setSizePolicy(sizePolicy);
        lineEditAxX0->setMinimumSize(QSize(25, 25));
        lineEditAxX0->setMaximumSize(QSize(78, 25));
        lineEditAxX0->setFont(font3);
        lineEditAxX0->setMaxLength(65500);
        lineEditAxX0->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditAxX0->setReadOnly(true);
        lineEditAxXsize = new QLineEdit(groupBox);
        lineEditAxXsize->setObjectName("lineEditAxXsize");
        lineEditAxXsize->setEnabled(true);
        lineEditAxXsize->setGeometry(QRect(143, 19, 78, 25));
        sizePolicy.setHeightForWidth(lineEditAxXsize->sizePolicy().hasHeightForWidth());
        lineEditAxXsize->setSizePolicy(sizePolicy);
        lineEditAxXsize->setMinimumSize(QSize(25, 25));
        lineEditAxXsize->setMaximumSize(QSize(78, 25));
        lineEditAxXsize->setFont(font3);
        lineEditAxXsize->setMaxLength(65500);
        lineEditAxXsize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditAxYmax = new QLineEdit(groupBox);
        lineEditAxYmax->setObjectName("lineEditAxYmax");
        lineEditAxYmax->setEnabled(true);
        lineEditAxYmax->setGeometry(QRect(362, 19, 78, 25));
        sizePolicy.setHeightForWidth(lineEditAxYmax->sizePolicy().hasHeightForWidth());
        lineEditAxYmax->setSizePolicy(sizePolicy);
        lineEditAxYmax->setMinimumSize(QSize(25, 25));
        lineEditAxYmax->setMaximumSize(QSize(78, 25));
        lineEditAxYmax->setFont(font3);
        lineEditAxYmax->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelXsize = new QLabel(groupBox);
        labelXsize->setObjectName("labelXsize");
        labelXsize->setGeometry(QRect(106, 19, 34, 25));
        labelXsize->setMinimumSize(QSize(25, 25));
        labelXsize->setMaximumSize(QSize(34, 25));
        QFont font5;
        font5.setPointSize(8);
        font5.setBold(false);
        labelXsize->setFont(font5);
        labelXsize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelYscale = new QLabel(groupBox);
        labelYscale->setObjectName("labelYscale");
        labelYscale->setGeometry(QRect(221, 19, 34, 25));
        labelYscale->setMinimumSize(QSize(34, 25));
        labelYscale->setMaximumSize(QSize(34, 25));
        labelYscale->setFont(font5);
        labelYscale->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushBPlotDefSave = new QPushButton(groupBox);
        pushBPlotDefSave->setObjectName("pushBPlotDefSave");
        pushBPlotDefSave->setEnabled(true);
        pushBPlotDefSave->setGeometry(QRect(442, 19, 60, 25));
        sizePolicy.setHeightForWidth(pushBPlotDefSave->sizePolicy().hasHeightForWidth());
        pushBPlotDefSave->setSizePolicy(sizePolicy);
        pushBPlotDefSave->setMinimumSize(QSize(25, 25));
        pushBPlotDefSave->setMaximumSize(QSize(60, 25));
        pushBPlotDefSave->setFont(font);
        pushBPlotDefSave->setLayoutDirection(Qt::LeftToRight);
        pushBPlotDefSave->setCheckable(false);
        pushBPlotDefSave->setFlat(false);
        pushBPlotDefReset = new QPushButton(groupBox);
        pushBPlotDefReset->setObjectName("pushBPlotDefReset");
        pushBPlotDefReset->setEnabled(true);
        pushBPlotDefReset->setGeometry(QRect(515, 19, 32, 25));
        sizePolicy.setHeightForWidth(pushBPlotDefReset->sizePolicy().hasHeightForWidth());
        pushBPlotDefReset->setSizePolicy(sizePolicy);
        pushBPlotDefReset->setMinimumSize(QSize(25, 25));
        pushBPlotDefReset->setMaximumSize(QSize(32, 25));
        pushBPlotDefReset->setFont(font);
        pushBPlotDefReset->setLayoutDirection(Qt::LeftToRight);
        pushBPlotDefReset->setIcon(icon);
        pushBPlotDefReset->setCheckable(false);
        pushBPlotDefReset->setFlat(false);
        labelX0 = new QLabel(groupBox);
        labelX0->setObjectName("labelX0");
        labelX0->setGeometry(QRect(4, 19, 20, 25));
        labelX0->setMinimumSize(QSize(20, 25));
        labelX0->setMaximumSize(QSize(20, 25));
        labelX0->setFont(font5);
        labelX0->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelMax = new QLabel(groupBox);
        labelMax->setObjectName("labelMax");
        labelMax->setGeometry(QRect(338, 19, 22, 25));
        labelMax->setMinimumSize(QSize(22, 25));
        labelMax->setMaximumSize(QSize(22, 25));
        labelMax->setFont(font);
        labelMax->setFrameShape(QFrame::NoFrame);
        labelMax->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutOpt->addWidget(groupBoxPlot, 0, 2, 1, 1);


        gridLayout->addLayout(gridLayoutOpt, 1, 0, 1, 1);

        QWidget::setTabOrder(comboBoxPort, comboBoxRate);
        QWidget::setTabOrder(comboBoxRate, comboBoxDataBits);
        QWidget::setTabOrder(comboBoxDataBits, comboBoxParity);
        QWidget::setTabOrder(comboBoxParity, comboBoxStopBits);
        QWidget::setTabOrder(comboBoxStopBits, comboBoxFlowControl);
        QWidget::setTabOrder(comboBoxFlowControl, spinBoxSync);
        QWidget::setTabOrder(spinBoxSync, pushBSerialPortDefSet);
        QWidget::setTabOrder(pushBSerialPortDefSet, checkBAutoSaveSet);
        QWidget::setTabOrder(checkBAutoSaveSet, checkBLaunchMax);
        QWidget::setTabOrder(checkBLaunchMax, checkBAutoNameFile);
        QWidget::setTabOrder(checkBAutoNameFile, checkBAutoScaleY);
        QWidget::setTabOrder(checkBAutoScaleY, checkBOGL);
        QWidget::setTabOrder(checkBOGL, lineEditAxX0);
        QWidget::setTabOrder(lineEditAxX0, lineEditAxXsize);
        QWidget::setTabOrder(lineEditAxXsize, lineEditAxYmin);
        QWidget::setTabOrder(lineEditAxYmin, lineEditAxYmax);
        QWidget::setTabOrder(lineEditAxYmax, pushBPlotDefSave);
        QWidget::setTabOrder(pushBPlotDefSave, pushBPlotDefReset);
        QWidget::setTabOrder(pushBPlotDefReset, lineEditPathData);
        QWidget::setTabOrder(lineEditPathData, pushBPathData);
        QWidget::setTabOrder(pushBPathData, lineEditPathLogs);
        QWidget::setTabOrder(lineEditPathLogs, pushBPathLogs);
        QWidget::setTabOrder(pushBPathLogs, pushBSetAllDef);
        QWidget::setTabOrder(pushBSetAllDef, pushBOk);
        QWidget::setTabOrder(pushBOk, pushBCancel);
        QWidget::setTabOrder(pushBCancel, textBrowserNoteReset);

        retranslateUi(Options);

        pushBCancel->setDefault(true);


        QMetaObject::connectSlotsByName(Options);
    } // setupUi

    void retranslateUi(QDialog *Options)
    {
        groupBoxPath->setTitle(QCoreApplication::translate("Options", "Paths", nullptr));
#if QT_CONFIG(tooltip)
        pushBPathLogs->setToolTip(QCoreApplication::translate("Options", "See and set path for log files", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBPathLogs->setStatusTip(QCoreApplication::translate("Options", "See and set path for log files", nullptr));
#endif // QT_CONFIG(statustip)
        pushBPathLogs->setText(QCoreApplication::translate("Options", "Browse", nullptr));
        lineEditPathData->setText(QString());
        label_data->setText(QCoreApplication::translate("Options", "Data files:", nullptr));
        label_logs->setText(QCoreApplication::translate("Options", "Logs files:", nullptr));
#if QT_CONFIG(tooltip)
        pushBPathData->setToolTip(QCoreApplication::translate("Options", "See and set path for data file", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBPathData->setStatusTip(QCoreApplication::translate("Options", "See and set path for data file", nullptr));
#endif // QT_CONFIG(statustip)
        pushBPathData->setText(QCoreApplication::translate("Options", "Browse", nullptr));
        groupBox_2->setTitle(QString());
#if QT_CONFIG(tooltip)
        pushBSetAllDef->setToolTip(QCoreApplication::translate("Options", "Reset aff to default values", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBSetAllDef->setStatusTip(QCoreApplication::translate("Options", "Reset aff to default values", nullptr));
#endif // QT_CONFIG(statustip)
        pushBSetAllDef->setText(QCoreApplication::translate("Options", "Set all defaults", nullptr));
#if QT_CONFIG(tooltip)
        pushBOk->setToolTip(QCoreApplication::translate("Options", "Apply and Close", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBOk->setStatusTip(QCoreApplication::translate("Options", "Apply and Close", nullptr));
#endif // QT_CONFIG(statustip)
        pushBOk->setText(QCoreApplication::translate("Options", "OK", nullptr));
#if QT_CONFIG(tooltip)
        pushBCancel->setToolTip(QCoreApplication::translate("Options", "Cancel and not set", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBCancel->setStatusTip(QCoreApplication::translate("Options", "Cancel and not set", nullptr));
#endif // QT_CONFIG(statustip)
        pushBCancel->setText(QCoreApplication::translate("Options", "Cancel", nullptr));
#if QT_CONFIG(tooltip)
        groupBoxSerial->setToolTip(QCoreApplication::translate("Options", "Set value for Serial Port", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        groupBoxSerial->setStatusTip(QCoreApplication::translate("Options", "Set value for Serial Port", nullptr));
#endif // QT_CONFIG(statustip)
        groupBoxSerial->setTitle(QCoreApplication::translate("Options", "Serial Port", nullptr));
        label_4->setText(QCoreApplication::translate("Options", "Port:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxPort->setToolTip(QCoreApplication::translate("Options", "Serial port", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        comboBoxPort->setStatusTip(QCoreApplication::translate("Options", "Serial port", nullptr));
#endif // QT_CONFIG(statustip)
        label->setText(QCoreApplication::translate("Options", "Baud rate:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxRate->setToolTip(QCoreApplication::translate("Options", "Serial port speed", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        comboBoxRate->setStatusTip(QCoreApplication::translate("Options", "Serial port speed", nullptr));
#endif // QT_CONFIG(statustip)
        label_2->setText(QCoreApplication::translate("Options", "Data bits:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxDataBits->setToolTip(QCoreApplication::translate("Options", "Data Bits", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        comboBoxDataBits->setStatusTip(QCoreApplication::translate("Options", "Data Bits", nullptr));
#endif // QT_CONFIG(statustip)
        label_3->setText(QCoreApplication::translate("Options", "Parity:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxParity->setToolTip(QCoreApplication::translate("Options", "Parity", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        comboBoxParity->setStatusTip(QCoreApplication::translate("Options", "Parity", nullptr));
#endif // QT_CONFIG(statustip)
        comboBoxParity->setCurrentText(QString());
        label_5->setText(QCoreApplication::translate("Options", "Stop bits:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxStopBits->setToolTip(QCoreApplication::translate("Options", "Stop Bits", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        comboBoxStopBits->setStatusTip(QCoreApplication::translate("Options", "Parity", nullptr));
#endif // QT_CONFIG(statustip)
        label_6->setText(QCoreApplication::translate("Options", "Flow cont.:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxFlowControl->setToolTip(QCoreApplication::translate("Options", "Flow Control", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        comboBoxFlowControl->setStatusTip(QCoreApplication::translate("Options", "Flow Control", nullptr));
#endif // QT_CONFIG(statustip)
        comboBoxFlowControl->setCurrentText(QString());
        labelSpin->setText(QCoreApplication::translate("Options", "Sync. time:", nullptr));
#if QT_CONFIG(tooltip)
        spinBoxSync->setToolTip(QCoreApplication::translate("Options", "Serial port timeout (ms)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        spinBoxSync->setStatusTip(QCoreApplication::translate("Options", "Serial port timeout (ms)", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        spinBoxSync->setWhatsThis(QCoreApplication::translate("Options", "Serial port timeout (ms)", nullptr));
#endif // QT_CONFIG(whatsthis)
        spinBoxSync->setSuffix(QCoreApplication::translate("Options", " ms", nullptr));
#if QT_CONFIG(tooltip)
        pushBSerialPortDefSet->setToolTip(QCoreApplication::translate("Options", "Set defaults value for Serial Port", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBSerialPortDefSet->setStatusTip(QCoreApplication::translate("Options", "Set defaults value for Serial Port", nullptr));
#endif // QT_CONFIG(statustip)
        pushBSerialPortDefSet->setText(QString());
        groupBoxPlot->setTitle(QCoreApplication::translate("Options", "Plotter settings", nullptr));
#if QT_CONFIG(tooltip)
        checkBAutoSaveSet->setToolTip(QCoreApplication::translate("Options", "Auto save settings", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        checkBAutoSaveSet->setStatusTip(QCoreApplication::translate("Options", "Auto save settings", nullptr));
#endif // QT_CONFIG(statustip)
        checkBAutoSaveSet->setText(QCoreApplication::translate("Options", "Auto save settings", nullptr));
        checkBLaunchMax->setText(QCoreApplication::translate("Options", "Launch maximized", nullptr));
#if QT_CONFIG(tooltip)
        checkBAutoNameFile->setToolTip(QCoreApplication::translate("Options", "Auto set name file for save data [brga_YYMMDD-hhmmss]", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        checkBAutoNameFile->setStatusTip(QCoreApplication::translate("Options", "Auto save settings", nullptr));
#endif // QT_CONFIG(statustip)
        checkBAutoNameFile->setText(QCoreApplication::translate("Options", "Auto Name for data file", nullptr));
#if QT_CONFIG(tooltip)
        checkBAutoScaleY->setToolTip(QCoreApplication::translate("Options", "Auto scale", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        checkBAutoScaleY->setStatusTip(QCoreApplication::translate("Options", "Auto scale", nullptr));
#endif // QT_CONFIG(statustip)
        checkBAutoScaleY->setText(QCoreApplication::translate("Options", "Auto scale plotter axis Y", nullptr));
        checkBOGL->setText(QCoreApplication::translate("Options", "Use OpenGL", nullptr));
        textBrowserNoteReset->setMarkdown(QCoreApplication::translate("Options", "The reset button sets default values for the X and Y axes:\n"
"\n"
"", nullptr));
        textBrowserNoteReset->setHtml(QCoreApplication::translate("Options", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; color:#000000;\">The reset button sets default values for the X and Y axes:</span></p></body></html>", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Options", "Default values for Plotter:", nullptr));
#if QT_CONFIG(tooltip)
        lineEditAxYmin->setToolTip(QCoreApplication::translate("Options", "min value axis Y", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEditAxYmin->setStatusTip(QCoreApplication::translate("Options", "min value axis Y", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        lineEditAxX0->setToolTip(QCoreApplication::translate("Options", "min value axis Y", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEditAxX0->setStatusTip(QCoreApplication::translate("Options", "min value axis Y", nullptr));
#endif // QT_CONFIG(statustip)
        lineEditAxX0->setText(QCoreApplication::translate("Options", "0", nullptr));
#if QT_CONFIG(tooltip)
        lineEditAxXsize->setToolTip(QCoreApplication::translate("Options", "min value axis Y", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEditAxXsize->setStatusTip(QCoreApplication::translate("Options", "min value axis Y", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        lineEditAxYmax->setToolTip(QCoreApplication::translate("Options", "max value axis Y", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEditAxYmax->setStatusTip(QCoreApplication::translate("Options", "max value axis Y", nullptr));
#endif // QT_CONFIG(statustip)
        labelXsize->setText(QCoreApplication::translate("Options", "Xsize", nullptr));
        labelYscale->setText(QCoreApplication::translate("Options", "Y min", nullptr));
#if QT_CONFIG(tooltip)
        pushBPlotDefSave->setToolTip(QCoreApplication::translate("Options", "Reset to defaults value for all axises", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBPlotDefSave->setStatusTip(QCoreApplication::translate("Options", "Reset to defaults value for all axises", nullptr));
#endif // QT_CONFIG(statustip)
        pushBPlotDefSave->setText(QCoreApplication::translate("Options", "Apply", nullptr));
#if QT_CONFIG(tooltip)
        pushBPlotDefReset->setToolTip(QCoreApplication::translate("Options", "Reset to defaults value for Plotter", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBPlotDefReset->setStatusTip(QCoreApplication::translate("Options", "Reset to defaults value for Plotter", nullptr));
#endif // QT_CONFIG(statustip)
        pushBPlotDefReset->setText(QString());
        labelX0->setText(QCoreApplication::translate("Options", "X0", nullptr));
        labelMax->setText(QCoreApplication::translate("Options", "max", nullptr));
        (void)Options;
    } // retranslateUi

};

namespace Ui {
    class Options: public Ui_Options {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONS_H
