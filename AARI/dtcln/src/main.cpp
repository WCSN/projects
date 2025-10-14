#include "dtcln.h"
#include <QApplication>

int main(int argc, char *argv[]) 
{
  QApplication app(argc, argv);
  DTCln dtcln;

  dtcln.show();
  app.exec();
}
