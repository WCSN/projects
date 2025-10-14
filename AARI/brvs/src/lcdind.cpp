#include <QtGui>
#include "lcdind.h"

LCDIndicator::LCDIndicator(QWidget *parent,QString label) :
		QWidget(parent)
{
	d_lcdind = new QLCDNumber(7);
	d_lcdind->setSegmentStyle(QLCDNumber::Flat);
	d_lcdind->setSmallDecimalPoint(true);
	d_lcdind->setMinimumSize(340,100);
	d_lcdind->setMode(QLCDNumber::Dec);
	//d_lcdind->setBaseSize(220,160);

	fl = new QFormLayout(this);

	QLabel *lb = new QLabel(label,this);
	fl->addRow(lb);
	fl->addRow(d_lcdind);
	setValue("0.00");
}


void LCDIndicator::setValue(const QString &value)
{
	d_lcdind->display(value);
}


void LCDIndicator::setValue(double value)
{
	char buff[32];
	sprintf(buff,"%8.2f",value);
	QString str(buff);

	str.replace(QChar(','),QChar('.'));
	d_lcdind->display(str);
}
