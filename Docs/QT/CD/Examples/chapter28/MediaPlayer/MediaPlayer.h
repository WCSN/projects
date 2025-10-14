/* ======================================================================
**  MediaPlayer.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#ifndef _MediaPlayer_h_
#define _MediaPlayer_h_

#include <QWidget>

namespace Phonon {
class AudioOutput;
class MediaObject;
}

// ======================================================================
class MediaPlayer : public QWidget {
    Q_OBJECT
private:
    Phonon::AudioOutput* m_pao;
    Phonon::MediaObject* m_pmedia;

public:
    MediaPlayer(QWidget* pwgt = 0);

public slots:
    void slotLoad();
};
#endif  //_MediaPlayer_h_
