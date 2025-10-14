/* ======================================================================
**  Buttons.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _Buttons_h_
#define _Buttons_h_

#include <QGroupBox>

class QCheckBox;
class QRadioButton;

// ======================================================================
class Buttons : public QGroupBox {
    Q_OBJECT
private:
    QCheckBox*    m_pchk;
    QRadioButton* m_pradRed;
    QRadioButton* m_pradGreen;
    QRadioButton* m_pradBlue;

public:
    Buttons(QWidget* pwgt = 0);

public slots:
    void slotButtonClicked();
};

#endif  //_Buttons_h_
