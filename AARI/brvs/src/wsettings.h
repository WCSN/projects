
#include <qframe.h>
#include <QSettings>

#include "mainwindow.h"

class WSettings;

class WSettings: public QWidget
{
	Q_OBJECT;

public:
    WSettings( QWidget *parent = NULL, Ui_MainWindow * ui = NULL );
    ~WSettings();

private:
    Ui_MainWindow *uip;

public Q_SLOTS:
	void checkBoxSee(void);
	void dialSee(void);
	void horslSee(void);
	void setcolorled(void);

private:
	void writeSettings(const QString &nameset);
	void readSettings(const QString &nameset);
};
