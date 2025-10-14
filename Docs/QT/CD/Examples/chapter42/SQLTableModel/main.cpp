/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include <QtSql>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("addressbook");

    db.setUserName("elton"); 
    db.setHostName("epica"); 
    db.setPassword("password"); 
    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError();
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    if (!createConnection()) {
        return -1;
    }

    QTableView     view;
    QSqlTableModel model;

    model.setTable("addressbook");
    model.select();
    model.setEditStrategy(QSqlTableModel::OnFieldChange);
    
    view.setModel(&model);
    view.show();
    
    return app.exec();
}


