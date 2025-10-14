#include "dtsrv.h"
#include <QCoreApplication>

int main(int argc, char *argv[]) 
{
  QCoreApplication app(argc, argv);

  DTSrv DTSrv(1024);

  app.exec();
}
