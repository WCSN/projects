/*
 * CarBox.h
 *
 *  Created on: Feb 7, 2019
 *      Author: wocson
 */

#ifndef SRC_CARBOX_H_
#define SRC_CARBOX_H_

#include <QtCore>
#include <QtGui>
#include <QPushButton>
#include <QApplication>


class CarBox;

class CarBox: public QWidget
{
	Q_OBJECT

public:
	CarBox( QWidget * = NULL );
	virtual ~CarBox();

private:
	QPushButton *btnR;
	QPushButton *btnN;
	QPushButton *btnD;
	QPushButton *btn2;
	QPushButton *btn1;
	QPushButton *btnP;
	QPushButton *btnM;
	
};

#endif /* SRC_CARBOX_H_ */
