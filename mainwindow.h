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
    QChartView *createChartView(const QString &title, QValueAxis *axisX, QValueAxis *axisY);
    void insertDataToSeries(QSplineSeries *series1, QSplineSeries *series2, const QVector<QPointF> &dataPoints);

    void toReadReady();
public:

private slots:
#if 1
    void startRefun1();
    void startRefun2();
    void dealTimerFun(QTimer *timer);
    void recordQueryButtonFun();
#endif
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

