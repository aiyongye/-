#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "mainwindow.h"
#include <QVariant>
#include <QMessageBox>

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
public:
        QSqlDatabase database;
        QSqlDatabase getDatabaseConnection(const QString &dbName);
        bool queryAllDataFromTable(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList);
        MainWindow w1;
        QList<QList<QVariant>> dataList;
        void findTableToConBox();
private:
    Ui::Login *ui;
};

#endif // LOGIN_H
