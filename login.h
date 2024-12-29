#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "mainwindow.h"
#include <QVariant>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>

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

        QList<QList<QVariant>> dataList;
        void findTableToConBox();
        //-------------bash20241215--------------
        void showLoginPage();
        void startResourceLoading();
        void loadW1ResourcesInBackground();
        void onLoginSuccess();
        MainWindow *w1; // 修改为指针，延迟创建
        MainWindow *w2;
public slots:

        //-------------bash20241215--------------
private:
    Ui::Login *ui;
};

#endif // LOGIN_H
