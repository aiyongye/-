#ifndef LOGINSET_H
#define LOGINSET_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QFile>
#include <QStringList>
#include <QFontDatabase>
#include <QListView>
#include <QDateTime>

namespace Ui {
class LoginSet;
}

class LoginSet : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginSet(QWidget *parent = 0);
    ~LoginSet();

private:
    Ui::LoginSet *ui;
public:
    QSqlDatabase database;
    QList<QList<QVariant>> dataList; //存储查询到的所有字段
    QList<QVariant> rowData; //存储单条消息
public:
    QSqlDatabase getDatabaseConnection(const QString &dbName);
    bool ensureTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName);
    bool insertDataIntoTable(QSqlDatabase &db, const QString &tableName, const QString &xuanName, const QString &pressStd, const QString &pressDate);
    bool queryAllDataFromTable(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList);
    bool deleteDataFromDatabase(QSqlDatabase &db, const QString &tableName, const QList<QVariant> &rowData);
};

#endif // LOGINSET_H
