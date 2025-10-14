#include <QtGui>
#include <QtCore>
#include <QLabel>
#include <qwt_dial_needle.h>
#include <qwt_analog_clock.h>
#include <qwt_round_scale_draw.h>

class Clock: public QwtAnalogClock
{
Q_OBJECT

public:
	Clock(QWidget *parent = NULL);

	void setLabel(const QString &);
	QString label() const;

protected:
	virtual void drawScaleContents(QPainter *painter, const QPointF &center,
			double radius) const;

private:
	QString d_label;
};


class ClockfTBar: public QLabel
{
Q_OBJECT

public:
	ClockfTBar(QWidget* pwgt = 0);

private Q_SLOTS:

	void slotUpdateDateTime();
};
