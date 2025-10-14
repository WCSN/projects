/* ======================================================================
**  test.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <QtTest>
#include <QtGui>

// ======================================================================
class Test_QLineEdit : public QObject {
Q_OBJECT
private slots:
    void edit();
};

// ----------------------------------------------------------------------
void Test_QLineEdit::edit()
{
    QLineEdit txt;
    QTest::keyClicks(&txt, "ABCDEFGH");

    QCOMPARE(txt.text(), QString("ABCDEFGH"));
    QVERIFY(txt.isModified());
}

QTEST_MAIN(Test_QLineEdit)
#include "test.moc"
