#include <QtGui>
#include <QFormLayout>
#include <QLCDNumber>
#include <QLabel>

class LCDIndicator: public QWidget
{
	Q_OBJECT

public:
    LCDIndicator(QWidget* parent = NULL,QString label = NULL );

    void setValue( const QString &);
    void setValue( double );
    void setValue( int );

private:
    QFormLayout *fl;
    QLCDNumber *d_lcdind;
};
