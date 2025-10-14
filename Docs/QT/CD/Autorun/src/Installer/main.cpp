/* ======================================================================
**  main.cpp
** ======================================================================
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include "Installer.h"

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load(":/resources/Installer_ru");
    app.installTranslator(&translator);

    Installer installer;
    installer.setWindowIcon(QPixmap(":/resources/icon.png"));
    installer.setWindowTitle(QObject::tr("Qt4 Book Installer"));
    installer.show();
    installer.setFixedSize(installer.size());
    installer.move((QApplication::desktop()->width() - installer.width()) / 2, 
                   (QApplication::desktop()->height() - installer.height()) / 2
                  );

    return app.exec();
}
