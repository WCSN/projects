#include "mainwindow.h"
#include <QApplication>
#include <QtCore>
#include <QtGui>
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Settings settings;

    MainWindow w;
    w.showMaximized();

  return app.exec();
}
