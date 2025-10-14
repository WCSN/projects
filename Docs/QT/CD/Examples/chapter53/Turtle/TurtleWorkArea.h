/* ======================================================================
**  TurtleWorkArea.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _TurtleWorkArea_h_
#define _TurtleWorkArea_h_

#include <QWidget>
#include <QScriptEngine>

class QTextEdit;
class Turtle;

// ======================================================================
class TurtleWorkArea : public QWidget {
Q_OBJECT
private:
    QTextEdit*    m_ptxt;
    QScriptEngine m_scriptEngine;
    Turtle*       m_pTurtle;

public:
    TurtleWorkArea(QWidget* pwgt = 0);

private slots:
    void slotEvaluate (   );
    void slotApplyCode(int);
};
#endif //_TurtleWorkArea_h_
