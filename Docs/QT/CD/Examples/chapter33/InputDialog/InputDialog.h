/* ======================================================================
**  InputDialog.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _InputDialog_h_
#define _InputDialog_h_

#include <QDialog>

class QLineEdit;
// ======================================================================
class InputDialog : public QDialog {
    Q_OBJECT
private:
    QLineEdit* m_ptxtFirstName;
    QLineEdit* m_ptxtLastName;

public:
    InputDialog(QWidget* pwgt = 0);

    QString firstName() const;
    QString lastName () const;
};
#endif  //_InputDialog_h_
