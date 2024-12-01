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
    static QList<QList<QVariant>>  queryTable(QSqlDatabase &db, const QString &dbName, const QString &tableName,
                                                  const QStringList &columns = QStringList("*"),
                                                  const QString &condition = QString());
    static bool queryAllDataFromTable(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList);
    static bool queryAllDataFromTableXuan(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList);
    bool streetTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName);
    bool insertStreetDataToTable(QSqlDatabase &db, const QString &dbName, const QString &tableName,
                                                const QString &streetData, const QString &pressDate,
                                                const QString &jieDianSign, const QString &leftData);
};

#endif // SQLITEACTION_H
