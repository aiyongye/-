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
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QPainter>
#include "poppler-qt5.h"
#include<QPdfWriter>
#include <QTextDocument>
#include <QFile>
#include "qdebugloginfo.h"

#include <QTextDocument>
#include <QTextBlock>
#include <QSettings>
#include <historyform1.h>
#include <QMessageBox>

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
    void printPreviewChart(QPrinter *printer);
    void filePrintPreviewChart();
    void printPreviewMain(QPrinter *printer);
    void filePrintPreviewMain();

        bool queryAllDataFromTableXuan(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList) ;
        QString getLastRecordId(QSqlDatabase &db, const QString &tableName, const QList<QVariant> &rowData, const QStringList &columnNames);
        void highlightMatchingRow(const QString &buf);
            bool queryDataByBuf(QSqlDatabase &db, const QString &tableName, QList<QVariant> &chartsData, const QString &buf);
public:
 QSqlDatabase dataBaseConn;
private slots:
      // 菜单项对应的槽函数
          void onOption1() ;

          void onOption2();

          void onOption3();

          void printOnOption1();
          void printonOption2() ;

          void loadTable2(QTableWidget *tableWidget);
          void exportPdf();
          void writePdf();
          void addTable(int rows, int cols, QList<int> colWidth, const QList<QList<QString> > &values);
public:
          QList<QList<QVariant>> dataList2; // 存储悬挂名称和标准数据库中的值
          QList<QList<QVariant>> dataList4; // 存储曲线记录
          QList<QList<QVariant>> mainJiLuList; // 存储主记录
          QList<QList<QVariant>> dateFind; // 按时间查询主记录
          QList<QList<QVariant>> dateFind2; // 按时间查询曲线记录
          QList<QVariant> rowData; //存储单条主记录消息
          QList<QVariant> chartsData; // 存储单条曲线数据记录消息
public:
          QString m_html;
signals:
          void dataUpdated(QList<QVariant> mainData, QList<QVariant> chartsData);

public:
          /***********************bash20241218***********************/
          void createDirectory(const QString& path);
          /***********************bash20241218***********************/

          /***********************bash20241227***********************/
          historyForm1 w1;
          bool updateDataInTable(QSqlDatabase &db, const QString &tableName, int rowId, const QString &newUserName, const QString &newPassword);
          bool deleteDataByIdAndReference(QSqlDatabase &db, const QString &tableName, const QString &refTableName, int rowId);
          /***********************bash20241227***********************/

          // 20241229
          void exportPdfChar();
          QString generateSimpleSvgChart();
          QString generateSimpleSvgChart2();
          void saveSvgChartAsImage();
          void saveSvgChartAsImage2();

          QList<QList<QVariant>> leftList;
          QList<QList<QVariant>> rightList;

          // 20241229

          // 20241230
          QList<QList<QVariant>> queryTableDate2(QSqlDatabase &db, const QString &dbName, const QString &tableName,
                                                             const QString &startDateTime, const QString &endDateTime,
                                                             const QString &xuanName, const QStringList &columns = QStringList("*"));
          QList<QList<QVariant>> queryTableDate3(QSqlDatabase &db, const QString &dbName, const QString &tableName,
                                                             const QString &startDateTime, const QString &endDateTime,
                                                             const QString &xuanName, const QStringList &columns = QStringList("*"));
          // 20241230
};

#endif // HSTORYLIST_H
