#ifndef SQLITEACTION_H
#include "mainwindow.h"
#define SQLITEACTION_H


class SqliteAction
{
public:
    SqliteAction();
    static QSqlDatabase getDatabaseConnection(const QString &dbName);
    static bool ensureTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName);
    static bool insertIntoTable(QSqlDatabase &db, const QString &dbName, const QString &tableName, const QList<QString> &columns,const QList<QVariant> &values);
};

#endif // SQLITEACTION_H
