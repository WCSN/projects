/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#include <QApplication>
#include <phonon>

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication        app(argc, argv);
    Phonon::MediaObject media;
    Phonon::AudioOutput ao(Phonon::MusicCategory);

    Phonon::createPath(&media, &ao);

    QObject::connect(&media, SIGNAL(finished()), &app, SLOT(quit()));

    media.setCurrentSource(Phonon::MediaSource("music/yesterday.wav"));
    media.play();

    return app.exec();
}
