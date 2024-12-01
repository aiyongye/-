#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QMainWindow>
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QComboBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableView>
#include <QChartView>
#include <QChart>
#include <QValueAxis>
#include <QSplineSeries>
#include <QCheckBox>
#include <QRadioButton>
#include <QDebug>
#include <QModbusDataUnit> //存储接收和发送数据的类，数据类型为1bit和16bit
#include <QModbusReply> //客户端访问服务器后得到的回复（如客户端读服务器数据时包含数据信息）
#include <QModbusTcpClient>
#include <QThread>
#include <QChartView> // 确保包含 QChartView
#include <QString>    // 如果用到了 QString
#include <QTimer>
#include <QElapsedTimer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "hstorylist.h"
#include "configset.h"
#include <QDateTimeAxis>
#include <QFile>
#include <wpdfwriter.h>

QT_CHARTS_USE_NAMESPACE // 如果使用Qt Charts模块
namespace Ui {
class MainWindow;
}
class QModbusClient;
class QModbusReply;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QModbusClient *modbusDevice;
public:
    QChartView *createChartView(const QString &title, QDateTimeAxis *axisX, QValueAxis *axisY);
    void startDataInsertion(QDateTimeAxis *axisX, QLineSeries *series, QChart *chart,QChartView *chartView);
QChartView* createChartView2(const QString &title, QDateTimeAxis *axisX, QValueAxis *axisY);
void startDataInsertion2(QDateTimeAxis *axisX, QLineSeries *series, QChart *chart, QChartView *chartView) ;

    void toReadReady();
    void clearChart(QChartView *chartView); // 清除表结构
    void stopTimers(); //清除定时器
    void applyStyles(QWidget *widget,QString stylesheet);
    void saveChartToImage(QChartView* chartView, const QString& filePath);
public:
QDateEdit *yaZhuangData;
QComboBox *xuanGuaName;
QComboBox *jianChaName;
QComboBox *caoZuoName;

QLineEdit *tuBianSet;

QTimer *timer; //时间节奏和plc频率一致
QElapsedTimer *elapsedTimer;
QVector<QPointF> data1; // 用于存储压装力值和时间
QPushButton *recordQueryButton; // 记录查询
QPushButton *startReBtn1;
QPushButton *startReBtn2;
QSqlDatabase dataBaseConn; //DB连接

QLineEdit *yaZhuangSaultLine1;
QLineEdit *xiuZhengLine;
QPushButton *xiuZhengBtn;
QLabel *xiuZhengLabel;
QSpinBox *saoMiaoData;
QPushButton *saveBtn;
QPushButton *saveTuBianBtn;
QLineEdit *jieDianSignLine1;
QPushButton *daYinChartBtn1;
QLineEdit *yaZhuang1;
QLineEdit *yaZhuangStdLine1;
QLineEdit *jieDianSignLine2;
QPushButton *daYinChartBtn2;
QLineEdit *yaZhuang2;
QLineEdit *yaZhuangSaultLine2;
QLineEdit *yaZhuangStdLine2;
QPushButton *loginButton;
QPushButton *logoutButton;
QPushButton *standardButton;
QPushButton *dataMaintenanceButton;
QPushButton *exitButton;
QLineEdit *zhanKaiLine;
QPushButton *zhanKaiBtn;
QLabel *qieHuan;

QMenuBar *menuBar;
// 创建主窗口的中心部件
QWidget *centralWidget;
// 主布局
QGridLayout *mainLayout;
// 添加工作模式分组
QGroupBox *workModeBox;

QRadioButton *putTongMode;
QRadioButton *weiHuMode;
QLabel *saoMiaoTime;
QLabel *danWeiMiao;
QLabel *tuBianSetLabel;
QGroupBox *recordCreationBox;

QCheckBox *shuJuBox;
QPushButton *jieShu1;
QPushButton *jieShu2;
// 图表区域
QGroupBox *chartBox;
QGridLayout *chartLayout;
// 添加控制区
QGroupBox *controlBox;

QDateTimeAxis *axisX1;
QDateTimeAxis *axisX2;
QValueAxis *axisY1;
QValueAxis *axisY2;
QLineSeries *series1;
QLineSeries *series2;
QChartView *chartView2;
QChartView *chartView1;
QTimer *timerChart02; //处理图表2
QTimer *timerChart01; //处理图表1
QChart *chart02;
//QChar *chart11;
QChart *chart;

private slots:
#if 1
    void startRefun1();
    void initializeControls();
    void initBuJu();
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

#endif
public:
    Ui::MainWindow *ui;
    //定义需要用到的定时器
    int Timer2;
    int Timer1; //处理图表1
public:
    int a;
    HstoryList w1;//历史界面
    ConfigSet w2; //工艺标准界面
    QList<QList<QVariant>> dataList; //存储operator数据库中的值
    QList<QList<QVariant>> dataList1; //存储检查者数据库中的值
    QList<QList<QVariant>> dataList2; //存储悬挂名称和标准数据库中的值
    QVector<int> chart1Container;// 当点击图表1开始时将压力值存储
    QVector<int> chart2Container;// 当点击图表2开始时将压力值存储
public:
//    void writePdf();
//void addTable(const QString &title, const QString &subTitle,
//                          int rows, int cols, QList<int> colWidth, const QList<QList<QString> > &values);
   void exportPdf();
private:
     QString m_html;  // 用于存储生成的 HTML 内容
     QPdfWriter *m_pdfWriter;

};

#endif // MAINWINDOW_H


