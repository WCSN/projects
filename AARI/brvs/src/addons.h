/*
 * addons.h
 *
 *  Created on: Jan 29, 2019
 *      Author: wocson
 */

#ifndef SRC_ADDONS_H_
#define SRC_ADDONS_H_

#include <QLabel>
#include <stddef.h>

double randomW(int first, int last);

class WLabel: public QLabel
{
	Q_OBJECT

private:
	bool m_bBlink;
	QString m_strText;

protected:
	virtual void timerEvent(QTimerEvent*);

public:
	WLabel(const QString& strText = NULL, QWidget* pwgt = NULL,
			const QColor &tcolor=-1, const QColor &bcolor=-1, int nInterval = 0);
};


#endif /* SRC_ADDONS_H_ */
