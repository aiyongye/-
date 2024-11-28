#ifndef HSTORYLIST_H
#include <QMainWindow>
#include <QTableWidget>
#include "hoveredrowitemdelegate.h"
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>  // 确保包含此头文件
#define HSTORYLIST_H

namespace Ui {
class HstoryList;
}

class HstoryList : public QMainWindow
{
    Q_OBJECT

public:
    explicit HstoryList(QWidget *parent = 0);
    ~HstoryList();

private:
    Ui::HstoryList *ui;
    void loadTable(QTableWidget *tableWidget);
    void appendOneRow(QTableWidget *tableWidget,QString x1, QString x2, QString x3,
                                QString x4, QString x5, QString x6,QString x7, QString x8, QString x9,
                                QString x10, QString x11, QString x12) ;
    void applyStyles(QWidget *widget,QString stylesheet);
public:
      static QSqlDatabase getDatabaseConnection(const QString &dbName);
      static bool ensureTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName);
      static bool insertIntoTable(QSqlDatabase &db, const QString &dbName, const QString &tableName,
                                  const QList<QString> &columns,const QList<QVariant> &values);
      static QList<QList<QVariant>>  queryTable(QSqlDatabase &db, const QString &dbName, const QString &tableName,
                                                    const QStringList &columns = QStringList("*"),
                                                    const QString &condition = QString());
      QList<QList<QVariant>> queryTableDate(QSqlDatabase &db, const QString &dbName, const QString &tableName,
                                                     const QString &startDateTime, const QString &endDateTime,const QStringList &columns = QStringList("*")) ;
public:
 QSqlDatabase dataBaseConn;
private slots:
      // 菜单项对应的槽函数
          void onOption1() ;

          void onOption2();

          void onOption3() ;
          void onOption4() ;
          void printOnOption1();
          void printonOption2() ;
};

#endif // HSTORYLIST_H
