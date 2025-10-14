/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <QtGui>
#include <QtSvg>

// ----------------------------------------------------------------------
int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QSvgWidget svg(":/motion.svg");
    svg.show();

    QObject::connect(svg.renderer(), SIGNAL(repaintNeeded()), 
                     &svg,           SLOT(repaint())
                    );

    return app.exec();
}
