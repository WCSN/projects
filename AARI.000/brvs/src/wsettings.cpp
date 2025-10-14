#include "wsettings.h"

WSettings::WSettings( QWidget *parent, Ui_MainWindow *ui ) :
		QWidget(parent), uip(ui)
{
	//readSettings("wsettings");
	setcolorled();
}

void WSettings::setcolorled( void )
{
	if(uip->radioBRed->isChecked()) uip->kled->setColor(QColor(Qt::red));
	else if(uip->radioBGreen->isChecked()) uip->kled->setColor(QColor(Qt::green));
	else if(uip->radioBBlue->isChecked()) uip->kled->setColor(QColor(Qt::blue));
}

void WSettings::checkBoxSee( void )
{
	int a = 0;
	if(uip->checkBox->checkState() == Qt::Checked) a = 2;
	else a = 3;
}

void WSettings::writeSettings( const QString &nameset )
{
	QSettings settings("Wocson L", "BR Client");
	settings.beginGroup(nameset);
	settings.setValue("size", size());

	settings.setValue("colorRed", uip->radioBRed->isChecked());
	settings.setValue("colorGreen", uip->radioBGreen->isChecked());
	settings.setValue("colorBlue", uip->radioBBlue->isChecked());

	settings.endGroup();
}

WSettings::~WSettings()
{
	writeSettings("wsettings");
}

void WSettings::readSettings( const QString &nameset )
{
	QSettings settings("Wocson L", "BR Client");
	settings.beginGroup(nameset);
	resize(settings.value("size", QSize(480, 360)).toSize());
	uip->radioBBlue->setChecked(settings.value("RBRed").toBool());
	uip->radioBBlue->setChecked(settings.value("RBGre").toBool());
	uip->radioBBlue->setChecked(settings.value("RBBlu").toBool());

	settings.endGroup();
}

void WSettings::dialSee( void )
{
	int a = uip->kNobDip->value();
	uip->label_4->setNum(uip->kNobDip->value());
}

void WSettings::horslSee( void )
{
	int a = uip->horizontalSlider->value();
	uip->label_3->setNum(uip->horizontalSlider->value());

}
