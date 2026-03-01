#include "connection.h"
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

Connection* Connection::instance = nullptr;

Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("oussama");
    db.setPassword("0000");

    if (db.open())
    {
        qDebug() << "Connected!";
        QMessageBox::information(nullptr, "Base de données", "Connected!");
    }
    else
    {
        const QString err = db.lastError().text();
        qDebug() << "Connection failed:" << err;
        QMessageBox::critical(nullptr, "Base de données", "Connection failed: " + err);
    }
}

Connection* Connection::getInstance()
{
    if (!instance)
        instance = new Connection();
    return instance;
}

QSqlDatabase Connection::getDatabase()
{
    return db;
}
