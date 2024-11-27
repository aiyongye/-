#include "mainwindow.h"

// 创建图表1
QChartView* MainWindow::createChartView(const QString &title, QDateTimeAxis *axisX, QValueAxis *axisY) {
    QChart *chart = new QChart();
    chart->setTitle(title);

    // 创建曲线数据系列
    QLineSeries *series = new QLineSeries();
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    series->append(currentTime, 0);  // 使用当前时间作为初始数据点

    chart->addSeries(series);
    chart->createDefaultAxes();  // 创建默认坐标轴

    // 设置X轴为日期时间轴
    axisX->setFormat("yyyy-MM-dd HH:mm:ss");  // 设置日期时间格式
    axisX->setTickCount(2);  // 设置 X 轴刻度数量（显示两个刻度，即当前时间和下一秒）

    // 设置初始X轴范围，确保显示当前时间及之前5秒的数据
    axisX->setMin(QDateTime::fromMSecsSinceEpoch(currentTime - 5000));  // 设置最小时间，5秒前
    axisX->setMax(QDateTime::fromMSecsSinceEpoch(currentTime + 1000));  // 设置最大时间，当前时间 + 1秒

    // 设置Y轴范围和刻度
    axisY->setRange(0, 450);        // 设置Y轴范围
    axisY->setTickCount(6);         // 设置Y轴刻度数量
    axisY->setLabelFormat("%.0f");  // 设置Y轴标签格式，显示整数

    // 将坐标轴应用到图表
    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);

    // 创建并返回QChartView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿

    startDataInsertion(axisX, series, chart, chartView);  // 传入chartView，确保更新
    return chartView;
}

void MainWindow::startDataInsertion(QDateTimeAxis *axisX, QLineSeries *series, QChart *chart, QChartView *chartView) {
    if (!timerChart01) {
        timerChart01 = new QTimer(this);  // 只创建一次定时器

        connect(timerChart01, &QTimer::timeout, this, [=]() {
            qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();  // 获取当前时间戳（毫秒）

            // 将全局变量a插入到曲线中，currentTime作为X值，a作为Y值
            series->append(currentTime, a);
            qDebug() << "Inserted data point: " << currentTime << ", " << a;

            // 最多显示4个数据点，删除最早的点
            if (series->count() > 4) {
                series->remove(0);  // 删除最早的数据点
            }

            // 获取当前数据点的数量
            int pointCount = series->count();
            qint64 firstPointTime = series->points().first().x();  // 获取最早的数据点的X值
            qint64 lastPointTime = series->points().last().x();    // 获取最新的数据点的X值

            // 动态更新X轴范围
            axisX->setMin(QDateTime::fromMSecsSinceEpoch(firstPointTime));  // 设置最小时间为最早的数据点
            axisX->setMax(QDateTime::fromMSecsSinceEpoch(lastPointTime));   // 设置最大时间为最新的数据点

            // 根据数据点数目动态设置X轴的刻度数
            axisX->setTickCount(qMin(4, pointCount));  // 显示2到4个X轴时间点

            // 强制更新图表视图
            chart->update();
            chartView->repaint();  // 强制重绘，更新图表视图
        });

    }
}

//tu 2
// 创建图表2
// 创建图表2
QChartView* MainWindow::createChartView2(const QString &title, QDateTimeAxis *axisX, QValueAxis *axisY) {
    QChart *chart = new QChart();
    chart->setTitle(title);

    // 创建曲线数据系列
    QLineSeries *series = new QLineSeries();
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    series->append(currentTime, 0);  // 使用当前时间作为初始数据点

    chart->addSeries(series);
    chart->createDefaultAxes();  // 创建默认坐标轴

    // 设置X轴为日期时间轴
    axisX->setFormat("yyyy-MM-dd HH:mm:ss");  // 设置日期时间格式
    axisX->setTickCount(2);  // 设置 X 轴刻度数量（显示两个刻度，即当前时间和下一秒）

    // 设置初始X轴范围，确保显示当前时间及之前5秒的数据
    axisX->setMin(QDateTime::fromMSecsSinceEpoch(currentTime - 5000));  // 设置最小时间，5秒前
    axisX->setMax(QDateTime::fromMSecsSinceEpoch(currentTime + 1000));  // 设置最大时间，当前时间 + 1秒

    // 设置Y轴范围和刻度
    axisY->setRange(0, 450);        // 设置Y轴范围
    axisY->setTickCount(6);         // 设置Y轴刻度数量
    axisY->setLabelFormat("%.0f");  // 设置Y轴标签格式，显示整数

    // 将坐标轴应用到图表
    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);

    // 创建并返回QChartView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿

    startDataInsertion2(axisX, series, chart, chartView);  // 传入chartView，确保更新
    return chartView;
}

// 启动数据插入
void MainWindow::startDataInsertion2(QDateTimeAxis *axisX, QLineSeries *series, QChart *chart, QChartView *chartView) {
    // 保存参数供定时器使用
    axisX2 = axisX;
    series2 = series;
    chart02 = chart;
    chartView2 = chartView;

    // 如果定时器尚未启动，则启动定时器
    if (Timer2 == -1) {
        // 启动定时器，每秒触发一次
//        Timer2 = startTimer(1000);  // 启动定时器，1000毫秒（1秒）
        qDebug() << "定时器已启动!";
    }
}

// 定时器触发事件
void MainWindow::timerEvent(QTimerEvent *event) {
    // 判断是否是 Timer2 定时器
    if (event->timerId() == Timer2) {
        qDebug() << "定时器触发: 更新数据";

        if (series2 && axisX2 && chart02 && chartView2) {
            qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();  // 获取当前时间戳（毫秒）

            // 将全局变量 a 插入到曲线中，currentTime 作为 X 值，a 作为 Y 值
            series2->append(currentTime, a);
            qDebug() << "Inserted data point: " << currentTime << ", " << a;

            // 最多显示 4 个数据点，删除最早的点
            if (series2->count() > 4) {
                series2->remove(0);  // 删除最早的数据点
            }

            // 获取当前数据点的数量
            int pointCount = series2->count();
            qint64 firstPointTime = series2->points().first().x();  // 获取最早的数据点的 X 值
            qint64 lastPointTime = series2->points().last().x();    // 获取最新的数据点的 X 值

            // 动态更新 X 轴范围
            axisX2->setMin(QDateTime::fromMSecsSinceEpoch(firstPointTime));  // 设置最小时间为最早的数据点
            axisX2->setMax(QDateTime::fromMSecsSinceEpoch(lastPointTime));   // 设置最大时间为最新的数据点

            // 根据数据点数目动态设置 X 轴的刻度数
            axisX2->setTickCount(qMin(4, pointCount));  // 显示 2 到 4 个 X 轴时间点

            // 强制更新图表视图
            chart02->update();
            chartView2->repaint();  // 强制重绘，更新图表视图
        }
    }
}
