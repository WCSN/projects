/*
 * CarBox.cpp
 *
 *  Created on: Feb 7, 2019
 *      Author: wocson
 */

#include "carbox.h"


CarBox::CarBox(QWidget *parent):
		QWidget(parent)
{
	QWidget *gb = parent;

	QRect rpos = geometry();

	int left = rpos.left();
	int top = rpos.top();
	int width = rpos.width();
	int height = rpos.height();

	btnR = new QPushButton(gb);
	btnR->setObjectName(QString::fromUtf8("btnR"));
	btnR->setGeometry(QRect(75, 0, 26, 21));
	QPalette palette2;
	QBrush brush4(QColor(192, 0, 0, 255));
	brush4.setStyle(Qt::SolidPattern);
	palette2.setBrush(QPalette::Active, QPalette::WindowText, brush4);
	palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush4);
	QBrush brush5(QColor(0, 0, 0, 255));
	brush5.setStyle(Qt::SolidPattern);
	palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
	palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush5);
	QBrush brush6(QColor(105, 111, 102, 255));
	brush6.setStyle(Qt::SolidPattern);
	palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
	QBrush brush7(QColor(132, 132, 127, 255));
	brush7.setStyle(Qt::SolidPattern);
	palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush7);
	btnR->setPalette(palette2);
	QFont font;
	font.setBold(true);
	font.setWeight(75);
	btnR->setFont(font);
	btnR->setCheckable(true);
	btnR->setText("R");

	btnN = new QPushButton(gb);
	btnN->setObjectName(QString::fromUtf8("btnN"));
	btnN->setGeometry(QRect(100, 0, 26, 21));
	QPalette palette3;
	QBrush brush8(QColor(0, 128, 0, 255));
	brush8.setStyle(Qt::SolidPattern);
	palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush8);
	palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush8);
	palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush7);
	btnN->setPalette(palette3);
	btnN->setFont(font);
	btnN->setCheckable(true);
	btnN->setChecked(true);
	btnN->setText("N");

	btnD = new QPushButton(gb);
	btnD->setObjectName(QString::fromUtf8("btnD"));
	btnD->setGeometry(QRect(125, 0, 26, 21));
	btnD->setFont(font);
	btnD->setCheckable(true);
	btnD->setText("D");

	btn2 = new QPushButton(gb);
	btn2->setObjectName(QString::fromUtf8("btn2"));
	btn2->setGeometry(QRect(150, 0, 26, 21));
	btn2->setFont(font);
	btn2->setCheckable(true);
	btn2->setText("2");

	btn1 = new QPushButton(gb);
	btn1->setObjectName(QString::fromUtf8("btn1"));
	btn1->setGeometry(QRect(175, 0, 26, 21));
	btn1->setFont(font);
	btn1->setCheckable(true);
	btn1->setText("1");

	btnP = new QPushButton(gb);
	btnP->setObjectName(QString::fromUtf8("btnP"));
	btnP->setGeometry(QRect(112, 20, 26, 16));
	btnP->setFont(font);
	btnP->setCheckable(true);
	btnP->setText("+");

	btnM = new QPushButton(gb);
	btnM->setObjectName(QString::fromUtf8("btnM"));
	btnM->setGeometry(QRect(138, 20, 26, 16));
	btnM->setFont(font);
	btnM->setCheckable(true);
	btnM->setFlat(false);
	btnM->setText("-");
}

CarBox::~CarBox()
{

}

