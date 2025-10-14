/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGridLayout *layoutGridPlot;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_3;
    QLabel *labelSaveTo;
    QLineEdit *lineEditFileDataSave;
    QPushButton *pushBClean;
    QLabel *labelYscale;
    QPushButton *pushBScaleX;
    QPushButton *pushBScale0;
    QLabel *labelX0;
    QLineEdit *lineEditAxX0;
    QLabel *labelMax;
    QPushButton *pushBScaleY;
    QLabel *labelXsize;
    QLineEdit *lineEditAxYmin;
    QLineEdit *lineEditAxXsize;
    QLineEdit *lineEditAxYmax;
    QPushButton *pushBOptions;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_2;
    QPushButton *btnStart;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1020, 728);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(100);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1020, 700));
        QFont font;
        font.setPointSize(8);
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        MainWindow->setTabShape(QTabWidget::Triangular);
        MainWindow->setUnifiedTitleAndToolBarOnMac(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMaximumSize(QSize(16777215, 16777215));
        centralWidget->setSizeIncrement(QSize(1, 1));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(5, 5, 5, 5);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout->setContentsMargins(3, 3, 3, 3);
        layoutGridPlot = new QGridLayout();
        layoutGridPlot->setSpacing(0);
        layoutGridPlot->setObjectName("layoutGridPlot");
        layoutGridPlot->setSizeConstraint(QLayout::SetDefaultConstraint);

        gridLayout->addLayout(layoutGridPlot, 1, 0, 1, 4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName("groupBox_3");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy1);
        groupBox_3->setMinimumSize(QSize(772, 42));
        groupBox_3->setMaximumSize(QSize(772, 42));
        QFont font1;
        font1.setPointSize(8);
        font1.setBold(true);
        groupBox_3->setFont(font1);
        groupBox_3->setStyleSheet(QString::fromUtf8("border-top-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"));
        groupBox_3->setFlat(true);
        labelSaveTo = new QLabel(groupBox_3);
        labelSaveTo->setObjectName("labelSaveTo");
        labelSaveTo->setGeometry(QRect(559, 18, 26, 21));
        sizePolicy1.setHeightForWidth(labelSaveTo->sizePolicy().hasHeightForWidth());
        labelSaveTo->setSizePolicy(sizePolicy1);
        labelSaveTo->setMinimumSize(QSize(26, 21));
        labelSaveTo->setMaximumSize(QSize(26, 21));
        QFont font2;
        font2.setPointSize(8);
        font2.setBold(false);
        labelSaveTo->setFont(font2);
        labelSaveTo->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditFileDataSave = new QLineEdit(groupBox_3);
        lineEditFileDataSave->setObjectName("lineEditFileDataSave");
        lineEditFileDataSave->setEnabled(false);
        lineEditFileDataSave->setGeometry(QRect(588, 15, 145, 25));
        lineEditFileDataSave->setMinimumSize(QSize(145, 25));
        lineEditFileDataSave->setMaximumSize(QSize(145, 25));
        lineEditFileDataSave->setFont(font2);
        lineEditFileDataSave->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushBClean = new QPushButton(groupBox_3);
        pushBClean->setObjectName("pushBClean");
        pushBClean->setEnabled(false);
        pushBClean->setGeometry(QRect(527, 15, 30, 25));
        sizePolicy1.setHeightForWidth(pushBClean->sizePolicy().hasHeightForWidth());
        pushBClean->setSizePolicy(sizePolicy1);
        pushBClean->setMinimumSize(QSize(30, 25));
        pushBClean->setMaximumSize(QSize(30, 25));
        pushBClean->setFont(font);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/btnCL.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushBClean->setIcon(icon1);
        labelYscale = new QLabel(groupBox_3);
        labelYscale->setObjectName("labelYscale");
        labelYscale->setGeometry(QRect(245, 17, 28, 21));
        labelYscale->setMinimumSize(QSize(28, 21));
        labelYscale->setMaximumSize(QSize(28, 21));
        labelYscale->setFont(font2);
        labelYscale->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushBScaleX = new QPushButton(groupBox_3);
        pushBScaleX->setObjectName("pushBScaleX");
        pushBScaleX->setGeometry(QRect(211, 15, 30, 25));
        sizePolicy1.setHeightForWidth(pushBScaleX->sizePolicy().hasHeightForWidth());
        pushBScaleX->setSizePolicy(sizePolicy1);
        pushBScaleX->setMinimumSize(QSize(30, 25));
        pushBScaleX->setMaximumSize(QSize(30, 25));
        QFont font3;
        font3.setPointSize(20);
        pushBScaleX->setFont(font3);
        pushBScaleX->setLayoutDirection(Qt::LeftToRight);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/btnSX.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushBScaleX->setIcon(icon2);
        pushBScaleX->setCheckable(false);
        pushBScaleX->setFlat(false);
        pushBScale0 = new QPushButton(groupBox_3);
        pushBScale0->setObjectName("pushBScale0");
        pushBScale0->setGeometry(QRect(495, 15, 30, 25));
        sizePolicy1.setHeightForWidth(pushBScale0->sizePolicy().hasHeightForWidth());
        pushBScale0->setSizePolicy(sizePolicy1);
        pushBScale0->setMinimumSize(QSize(30, 25));
        pushBScale0->setMaximumSize(QSize(30, 25));
        pushBScale0->setFont(font);
        pushBScale0->setLayoutDirection(Qt::LeftToRight);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/btnS0.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushBScale0->setIcon(icon3);
        pushBScale0->setCheckable(false);
        pushBScale0->setFlat(false);
        labelX0 = new QLabel(groupBox_3);
        labelX0->setObjectName("labelX0");
        labelX0->setGeometry(QRect(0, 17, 18, 21));
        labelX0->setMinimumSize(QSize(18, 21));
        labelX0->setMaximumSize(QSize(18, 21));
        labelX0->setFont(font2);
        labelX0->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditAxX0 = new QLineEdit(groupBox_3);
        lineEditAxX0->setObjectName("lineEditAxX0");
        lineEditAxX0->setEnabled(true);
        lineEditAxX0->setGeometry(QRect(21, 15, 78, 25));
        sizePolicy1.setHeightForWidth(lineEditAxX0->sizePolicy().hasHeightForWidth());
        lineEditAxX0->setSizePolicy(sizePolicy1);
        lineEditAxX0->setMinimumSize(QSize(78, 25));
        lineEditAxX0->setMaximumSize(QSize(78, 25));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Liberation Mono")});
        font4.setPointSize(8);
        font4.setBold(false);
        lineEditAxX0->setFont(font4);
        lineEditAxX0->setMaxLength(65500);
        lineEditAxX0->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditAxX0->setReadOnly(false);
        labelMax = new QLabel(groupBox_3);
        labelMax->setObjectName("labelMax");
        labelMax->setGeometry(QRect(355, 17, 26, 21));
        labelMax->setMinimumSize(QSize(26, 21));
        labelMax->setMaximumSize(QSize(26, 64));
        labelMax->setFont(font);
        labelMax->setFrameShape(QFrame::NoFrame);
        labelMax->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushBScaleY = new QPushButton(groupBox_3);
        pushBScaleY->setObjectName("pushBScaleY");
        pushBScaleY->setGeometry(QRect(464, 15, 30, 25));
        sizePolicy1.setHeightForWidth(pushBScaleY->sizePolicy().hasHeightForWidth());
        pushBScaleY->setSizePolicy(sizePolicy1);
        pushBScaleY->setMinimumSize(QSize(30, 25));
        pushBScaleY->setMaximumSize(QSize(30, 25));
        pushBScaleY->setFont(font);
        pushBScaleY->setLayoutDirection(Qt::LeftToRight);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/btnSY.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushBScaleY->setIcon(icon4);
        pushBScaleY->setCheckable(false);
        pushBScaleY->setFlat(false);
        labelXsize = new QLabel(groupBox_3);
        labelXsize->setObjectName("labelXsize");
        labelXsize->setGeometry(QRect(102, 17, 28, 21));
        labelXsize->setMinimumSize(QSize(28, 21));
        labelXsize->setMaximumSize(QSize(28, 21));
        labelXsize->setFont(font);
        labelXsize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditAxYmin = new QLineEdit(groupBox_3);
        lineEditAxYmin->setObjectName("lineEditAxYmin");
        lineEditAxYmin->setEnabled(true);
        lineEditAxYmin->setGeometry(QRect(275, 15, 78, 25));
        sizePolicy1.setHeightForWidth(lineEditAxYmin->sizePolicy().hasHeightForWidth());
        lineEditAxYmin->setSizePolicy(sizePolicy1);
        lineEditAxYmin->setMinimumSize(QSize(78, 25));
        lineEditAxYmin->setMaximumSize(QSize(78, 25));
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Liberation Mono")});
        font5.setPointSize(8);
        lineEditAxYmin->setFont(font5);
        lineEditAxYmin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditAxXsize = new QLineEdit(groupBox_3);
        lineEditAxXsize->setObjectName("lineEditAxXsize");
        lineEditAxXsize->setEnabled(true);
        lineEditAxXsize->setGeometry(QRect(132, 15, 78, 25));
        sizePolicy1.setHeightForWidth(lineEditAxXsize->sizePolicy().hasHeightForWidth());
        lineEditAxXsize->setSizePolicy(sizePolicy1);
        lineEditAxXsize->setMinimumSize(QSize(78, 25));
        lineEditAxXsize->setMaximumSize(QSize(78, 25));
        lineEditAxXsize->setFont(font4);
        lineEditAxXsize->setMaxLength(65500);
        lineEditAxXsize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditAxYmax = new QLineEdit(groupBox_3);
        lineEditAxYmax->setObjectName("lineEditAxYmax");
        lineEditAxYmax->setEnabled(true);
        lineEditAxYmax->setGeometry(QRect(385, 15, 78, 25));
        sizePolicy1.setHeightForWidth(lineEditAxYmax->sizePolicy().hasHeightForWidth());
        lineEditAxYmax->setSizePolicy(sizePolicy1);
        lineEditAxYmax->setMinimumSize(QSize(78, 25));
        lineEditAxYmax->setMaximumSize(QSize(78, 25));
        lineEditAxYmax->setFont(font4);
        lineEditAxYmax->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushBOptions = new QPushButton(groupBox_3);
        pushBOptions->setObjectName("pushBOptions");
        pushBOptions->setGeometry(QRect(742, 15, 30, 25));
        pushBOptions->setMinimumSize(QSize(28, 25));
        pushBOptions->setMaximumSize(QSize(30, 25));
        pushBOptions->setFont(font2);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/btnOpt.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushBOptions->setIcon(icon5);
        pushBOptions->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(groupBox_3);

        horizontalSpacer_2 = new QSpacerItem(111, 1, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName("groupBox_2");
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        groupBox_2->setMinimumSize(QSize(84, 42));
        groupBox_2->setMaximumSize(QSize(84, 42));
        groupBox_2->setFont(font1);
        groupBox_2->setFlat(true);
        btnStart = new QPushButton(groupBox_2);
        btnStart->setObjectName("btnStart");
        btnStart->setGeometry(QRect(2, 12, 80, 30));
        sizePolicy1.setHeightForWidth(btnStart->sizePolicy().hasHeightForWidth());
        btnStart->setSizePolicy(sizePolicy1);
        btnStart->setMinimumSize(QSize(80, 30));
        btnStart->setMaximumSize(QSize(80, 30));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush1(QColor(173, 213, 217, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush2);
        QBrush brush3(QColor(214, 234, 236, 255));
        brush3.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Midlight, brush3);
        QBrush brush4(QColor(86, 106, 108, 255));
        brush4.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush4);
        QBrush brush5(QColor(115, 142, 145, 255));
        brush5.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush2);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Shadow, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush7(QColor(0, 0, 0, 127));
        brush7.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush7);
#endif
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        QBrush brush8(QColor(240, 240, 240, 255));
        brush8.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush8);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush2);
        QBrush brush9(QColor(227, 227, 227, 255));
        brush9.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Midlight, brush9);
        QBrush brush10(QColor(160, 160, 160, 255));
        brush10.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush10);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush10);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush8);
        QBrush brush11(QColor(105, 105, 105, 255));
        brush11.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Shadow, brush11);
        QBrush brush12(QColor(245, 245, 245, 255));
        brush12.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush12);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush13(QColor(0, 0, 0, 128));
        brush13.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush13);
#endif
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Midlight, brush3);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Shadow, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush12);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush13);
#endif
        btnStart->setPalette(palette);
        QFont font6;
        font6.setPointSize(9);
        font6.setBold(false);
        btnStart->setFont(font6);
        btnStart->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnStart->setAutoDefault(true);
        btnStart->setFlat(false);

        horizontalLayout->addWidget(groupBox_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 4);

        gridLayout->setRowStretch(1, 1);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        statusBar->setSizeGripEnabled(true);
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(lineEditAxX0, lineEditAxXsize);
        QWidget::setTabOrder(lineEditAxXsize, pushBScaleX);
        QWidget::setTabOrder(pushBScaleX, lineEditAxYmin);
        QWidget::setTabOrder(lineEditAxYmin, lineEditAxYmax);
        QWidget::setTabOrder(lineEditAxYmax, pushBScaleY);
        QWidget::setTabOrder(pushBScaleY, pushBScale0);
        QWidget::setTabOrder(pushBScale0, pushBClean);
        QWidget::setTabOrder(pushBClean, lineEditFileDataSave);
        QWidget::setTabOrder(lineEditFileDataSave, pushBOptions);
        QWidget::setTabOrder(pushBOptions, btnStart);

        retranslateUi(MainWindow);

        btnStart->setDefault(true);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "BRGA", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Plotter", nullptr));
        labelSaveTo->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(tooltip)
        lineEditFileDataSave->setToolTip(QCoreApplication::translate("MainWindow", "Data save file", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEditFileDataSave->setStatusTip(QCoreApplication::translate("MainWindow", "Data save to file", nullptr));
#endif // QT_CONFIG(statustip)
        lineEditFileDataSave->setText(QCoreApplication::translate("MainWindow", "brga-20230512-131319.csv", nullptr));
#if QT_CONFIG(tooltip)
        pushBClean->setToolTip(QCoreApplication::translate("MainWindow", "Clean Plotter", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBClean->setStatusTip(QCoreApplication::translate("MainWindow", "Clean Plotter", nullptr));
#endif // QT_CONFIG(statustip)
        pushBClean->setText(QString());
        labelYscale->setText(QCoreApplication::translate("MainWindow", "Y min", nullptr));
#if QT_CONFIG(tooltip)
        pushBScaleX->setToolTip(QCoreApplication::translate("MainWindow", "Scale axis X", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBScaleX->setStatusTip(QCoreApplication::translate("MainWindow", "Scale axis X", nullptr));
#endif // QT_CONFIG(statustip)
        pushBScaleX->setText(QString());
#if QT_CONFIG(tooltip)
        pushBScale0->setToolTip(QCoreApplication::translate("MainWindow", "Set defaults value for plot", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBScale0->setStatusTip(QCoreApplication::translate("MainWindow", "Set defaults value for plot", nullptr));
#endif // QT_CONFIG(statustip)
        pushBScale0->setText(QString());
        labelX0->setText(QCoreApplication::translate("MainWindow", "X0", nullptr));
#if QT_CONFIG(tooltip)
        lineEditAxX0->setToolTip(QCoreApplication::translate("MainWindow", "min value axis Y", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEditAxX0->setStatusTip(QCoreApplication::translate("MainWindow", "min value axis Y", nullptr));
#endif // QT_CONFIG(statustip)
        lineEditAxX0->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        labelMax->setText(QCoreApplication::translate("MainWindow", "max", nullptr));
#if QT_CONFIG(tooltip)
        pushBScaleY->setToolTip(QCoreApplication::translate("MainWindow", "Scale axis Y", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBScaleY->setStatusTip(QCoreApplication::translate("MainWindow", "Scale axis Y", nullptr));
#endif // QT_CONFIG(statustip)
        pushBScaleY->setText(QString());
        labelXsize->setText(QCoreApplication::translate("MainWindow", "Xsize", nullptr));
#if QT_CONFIG(tooltip)
        lineEditAxYmin->setToolTip(QCoreApplication::translate("MainWindow", "min value axis Y", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEditAxYmin->setStatusTip(QCoreApplication::translate("MainWindow", "min value axis Y", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        lineEditAxXsize->setToolTip(QCoreApplication::translate("MainWindow", "min value axis Y", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEditAxXsize->setStatusTip(QCoreApplication::translate("MainWindow", "min value axis Y", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        lineEditAxYmax->setToolTip(QCoreApplication::translate("MainWindow", "max value axis Y", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEditAxYmax->setStatusTip(QCoreApplication::translate("MainWindow", "max value axis Y", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        pushBOptions->setToolTip(QCoreApplication::translate("MainWindow", "Options", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pushBOptions->setStatusTip(QCoreApplication::translate("MainWindow", "Options", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        pushBOptions->setWhatsThis(QCoreApplication::translate("MainWindow", "Options", nullptr));
#endif // QT_CONFIG(whatsthis)
        pushBOptions->setText(QString());
        groupBox_2->setTitle(QString());
#if QT_CONFIG(tooltip)
        btnStart->setToolTip(QCoreApplication::translate("MainWindow", "Start/Stop receveiving data", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        btnStart->setStatusTip(QCoreApplication::translate("MainWindow", "Start/Stop receveiving data", nullptr));
#endif // QT_CONFIG(statustip)
        btnStart->setText(QCoreApplication::translate("MainWindow", "Start", "\320\241\321\202\320\260\321\200\321\202"));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
