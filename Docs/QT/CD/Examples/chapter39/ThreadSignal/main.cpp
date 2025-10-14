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

public:
    void run()
    {
        for (int i = 0; i <= 100; ++i) {
            usleep(100000);       
            emit progress(i);
        }
    }

signals:
    void progress(int);
};

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QProgressBar prb;
    MyThread     thread;

    QObject::connect(&thread, SIGNAL(progress(int)), 
                     &prb,    SLOT(setValue(int))
                    );

    prb.show();

    thread.start();
 
    return app.exec();
}

#include "main.moc"
