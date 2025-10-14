/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <QtGui>

// ----------------------------------------------------------------------
class Wizard : public QWizard { 
private:
    QWizardPage* createPage(QWidget* pwgt, QString strTitle)
    {
        QWizardPage* ppage = new QWizardPage;
        ppage->setTitle(strTitle);

        QVBoxLayout* playout = new QVBoxLayout;
        playout->addWidget(pwgt);
        ppage->setLayout(playout);

        return ppage;
    }

public: 
    Wizard::Wizard(QWidget* pwgt = 0) : QWizard(pwgt)  
    {
        addPage(createPage(new QLabel("<H1>Label 1</H1>"), "One")); 
        addPage(createPage(new QLabel("<H1>Label 2</H1>"), "Two")); 
        addPage(createPage(new QLabel("<H1>Label 3</H1>"), "Three")); 
    } 
};

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Wizard       wizardDialog;
    
    wizardDialog.show();

    return app.exec();
}
 

