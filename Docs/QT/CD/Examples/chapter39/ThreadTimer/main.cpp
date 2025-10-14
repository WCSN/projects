/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#include <QtGui>

// ======================================================================
class MyThread : public QThread {
Q_OBJECT
private:
    int m_nValue;

public:
    MyThread() : m_nValue(10)
    {
    }

    void run()
    {
        QTimer timer;
        connect(&timer, SIGNAL(timeout()), SLOT(slotNextValue()));
        timer.start(1000);

        exec();
    }

signals:
    void finished    (   );
    void currentValue(int);

public slots:
    void slotNextValue()
    {
        emit currentValue(--m_nValue);

        if (!m_nValue) {
            emit finished();
        }
        
    }
};

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QLCDNumber   lcd;
    MyThread     thread;

    QObject::connect(&thread, SIGNAL(currentValue(int)), 
                     &lcd,    SLOT(display(int))
                    );
    QObject::connect(&thread, SIGNAL(finished()), 
                     &app,    SLOT(quit())
                    );

    lcd.setSegmentStyle(QLCDNumber::Filled);
    lcd.display(10);
    lcd.resize(220, 90);
    lcd.show();
    thread.start();
 
    return app.exec();
}

#include "main.moc"
