#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

class Connection
{
private:
    static Connection* instance;
    QSqlDatabase db;
    Connection();

public:
    static Connection* getInstance();
    QSqlDatabase getDatabase();
};

#endif
