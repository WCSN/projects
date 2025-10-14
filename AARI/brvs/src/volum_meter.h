#include <qstring.h>
#include <qwt_dial.h>

class VolumMeter: public QwtDial
{

	Q_OBJECT
public:
    VolumMeter( QWidget *parent = NULL );

    void setLabel( const QString & );
    QString label() const;

    int shift = 1;

protected:
    virtual void drawScaleContents( QPainter *painter,
        const QPointF &center, double radius ) const;



private:
    QString d_label;
};
