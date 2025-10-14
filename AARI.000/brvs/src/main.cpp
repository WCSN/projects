#include "mainwindow.h"
#include <QApplication>
#include <QtCore>
#include <QtGui>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow w;

  w.show();//Maximized();
  
  return app.exec();
}
