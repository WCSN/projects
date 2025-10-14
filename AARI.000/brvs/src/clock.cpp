#include <qpainter.h>
#include <qwt_dial_needle.h>
#include <qwt_analog_clock.h>
#include <qwt_round_scale_draw.h>
#include "clock.h"

Clock::Clock( QWidget *parent ):
    QwtAnalogClock( parent ),
    d_label( "Honda" )
{
#if 0
      // disable minor ticks
	  QwtRoundScaleDraw *scaleDraw = new QwtRoundScaleDraw();
	  scaleDraw()->setTickLength( QwtScaleDiv::MinorTick, 0 );
#endif
      const QColor knobColor = QColor( Qt::white ).lighter( 130 );

      for ( int i = 0; i < QwtAnalogClock::NHands; i++ )
      {
          QColor handColor = QColor( Qt::green ).lighter( 150 );
          int width = 9;

          if ( i == QwtAnalogClock::SecondHand )
          {
              handColor = Qt::red;
              width = 3;
          }

          QwtDialSimpleNeedle *hand = new QwtDialSimpleNeedle(
          QwtDialSimpleNeedle::Arrow, true, handColor, knobColor );
          hand->setWidth( width );

          setHand( static_cast<QwtAnalogClock::Hand>( i ), hand );
      }
}

void Clock::setLabel( const QString &label )
{
    d_label = label;
    update();
}

QString Clock::label() const
{
    return d_label;
}

void Clock::drawScaleContents( QPainter *painter, const QPointF &center, double radius ) const
{
    QRectF rect( 0.0, 0.0, 2.0 * radius, 2.0 * radius - 10.0 );
    rect.moveCenter( center );

    const QColor color = palette().color( QPalette::Text );
    painter->setPen( color );

    const int flags = Qt::AlignBottom | Qt::AlignHCenter;
    painter->drawText( rect, flags, d_label );
}


ClockfTBar::ClockfTBar(QWidget* parent):
		QLabel(parent)
{
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateDateTime()));
	timer->start(500);
	slotUpdateDateTime();
}

void ClockfTBar::slotUpdateDateTime()
{
	QString str = QDateTime::currentDateTime().toString(
			Qt::SystemLocaleLongDate);//SystemLocaleDate);
	setText(str);
}

