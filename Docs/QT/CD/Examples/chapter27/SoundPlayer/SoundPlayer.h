/* ======================================================================
**  SoundPlayer.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _SoundPlayer_h_
#define _SoundPlayer_h_

#include <QWidget>

class QSound;

// ======================================================================
class SoundPlayer : public QWidget {
    Q_OBJECT
private:
    QSound* m_psnd;

public:
    SoundPlayer(QWidget* pwgt = 0);

public slots:
    void slotPlay       (    );
    void slotStop       (    );
    void slotLoopToggled(bool);
};
#endif  //_SoundPlayer_h_
