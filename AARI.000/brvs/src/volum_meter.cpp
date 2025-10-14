#include <qpainter.h>
#include <qwt_dial_needle.h>
#include <qwt_round_scale_draw.h>
#include <qevent.h>
#include "volum_meter.h"

VolumMeter::VolumMeter( QWidget *parent ):
    QwtDial( parent ),
    d_label( "x1000/min" )
{
    setOrigin( 135.0 );
    setScale( 0.0, 6.0 );
    setScaleArc( 0.0, 225.0 );
    setValue(0.0);

   	QwtRoundScaleDraw *scaleDraw = new QwtRoundScaleDraw();
    scaleDraw->setSpacing( 8 );
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Backbone, false );
    scaleDraw->setTickLength( QwtScaleDiv::MinorTick, 5 );
    scaleDraw->setTickLength( QwtScaleDiv::MediumTick, 4 );
    scaleDraw->setTickLength( QwtScaleDiv::MajorTick, 10 );
    setScaleDraw( scaleDraw );

    setWrapping( false );
    setReadOnly( true );

    QwtDialSimpleNeedle *needle = new QwtDialSimpleNeedle(
        QwtDialSimpleNeedle::Arrow, true, Qt::green,
        QColor( Qt::white).light( 130 ) );
    needle->setWidth(9.0);
    setNeedle( needle );
}

void VolumMeter::setLabel( const QString &label )
{
    d_label = label;
    update();
}

QString VolumMeter::label() const
{
    return d_label;
}

void VolumMeter::drawScaleContents( QPainter *painter,
    const QPointF &center, double radius ) const
{
    QRectF rect( 0.0, 0.0, 2.0 * radius, 2.0 * radius - 10.0);
    rect.moveCenter( center );

    const QColor color = palette().color( QPalette::Text );
    painter->setPen( color );

    const int flags = Qt::AlignBottom | Qt::AlignHCenter;
    painter->drawText( rect, flags, d_label );
}
