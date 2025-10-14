/* ======================================================================
**  Printer.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#ifndef _Printer_h_
#define _Printer_h_

#include <QWidget>

class QPrinter;
class QPaintDevice;

// ======================================================================
class Printer : public QWidget {
    Q_OBJECT

private:
    QPrinter* m_pprinter;

protected:
    virtual void paintEvent(QPaintEvent* pe  );
            void draw      (QPaintDevice* ppd);

public:
    Printer(QWidget* pwgt = 0);
    virtual ~Printer();

public slots:
    void slotPrint();
};
#endif  //_Printer_h_
