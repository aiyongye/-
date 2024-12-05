#ifndef PRESSSTD_H
#define PRESSSTD_H

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
#include "form1fix.h"
#include "qdebugloginfo.h"
namespace Ui {
class PressStd;
}

class PressStd : public QMainWindow
{
    Q_OBJECT

public:
    explicit PressStd(QWidget *parent = 0);
    ~PressStd();
public:
    QSqlDatabase getDatabaseConnection(const QString &dbName);
    bool ensureTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName);
    bool insertDataIntoTable(QSqlDatabase &db, const QString &tableName, const QString &xuanName, const QString &pressStd, const QString &pressDate);
    bool queryAllDataFromTable(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList);
    bool deleteDataFromDatabase(QSqlDatabase &db, const QString &tableName, const QList<QVariant> &rowData);
    bool updateDataInTable(QSqlDatabase &db, const QString &tableName, int rowId, const QString &newUserName, const QString &newPassword);
public:
        QSqlDatabase database;
        QList<QList<QVariant>> dataList; //存储查询到的所有字段
        QList<QVariant> rowData; //存储单条消息
        Form1Fix w1; // 修改弹窗
private:
    Ui::PressStd *ui;
        void applyStyles(QWidget *widget,QString stylesheet);
};

#endif // PRESSSTD_H
