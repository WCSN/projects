/*
 * addons.h
 *
 *  Created on: Jan 29, 2019
 *      Author: wocson
 */

#ifndef SRC_ADDONS_H_
#define SRC_ADDONS_H_

#include <QLabel>
#include <QColor>
#include <QString>
#include <QDateTime>

double randomW(int32_t first, int32_t last);

class WLabel: public QLabel
{
    Q_OBJECT

private:
    bool m_bBlink;
    int nInterval=0;
    QString m_strText;

protected:

    virtual void timerEvent(QTimerEvent*);

public:
    WLabel(const QString& strText = NULL, QWidget* pwgt = NULL,
            const QColor &tcolor=-1, const QColor &bcolor=-1, int nInterval = 0);

    void wsetColors(const QColor &tcolor, const QColor &bcolor);
    void wsetText(const QString &text);
};

QString CurrentTime(QString tmformat); // def "yyyyMMdd-hhmmss"

#endif /* SRC_ADDONS_H_ */
