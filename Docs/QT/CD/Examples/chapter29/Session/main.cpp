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
class MyApplication : public QApplication {
public:
    // ------------------------------------------------------------------
    MyApplication(int argc, char** argv)
        : QApplication(argc, argv)
    {
    }

    // ------------------------------------------------------------------
    virtual ~MyApplication()
    {
    }

    // ------------------------------------------------------------------
    virtual void commitData(QSessionManager&) {
        QMessageBox::information(0, 
                                 "Dialog", 
                                 "You are exiting operation system"
                                );
    }
};

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    MyApplication app(argc, argv);

    QLabel lbl("Exit you'r operation system to see the dialog box", 0);

    lbl.show();

    return app.exec();
}
