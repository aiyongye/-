#ifndef USERCREATE_H
#define USERCREATE_H

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
#include "form2_1fix.h"

namespace Ui {
class UserCreate;
}

class UserCreate : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserCreate(QWidget *parent = 0);
    ~UserCreate();

private:
    Ui::UserCreate *ui;
public:
    QSqlDatabase database;
    QList<QList<QVariant>> dataList; //存储查询到的所有字段
    QList<QVariant> rowData; //存储单条消息
    Form2_1Fix w1; // 弹窗修改
public:
    QSqlDatabase getDatabaseConnection(const QString &dbName);
    bool ensureTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName);
    bool insertDataIntoTable(QSqlDatabase &db, const QString &tableName, const QString &userName, const QString &upDate);
    bool queryAllDataFromTable(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList);
private slots:
    void onOption1();
    void onOption2();
};

#endif // USERCREATE_H
