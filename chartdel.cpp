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
    axisX->setLabelsFont(QFont("Arial", 10));  // 设置X轴的字体为Arial，大小为10
    axisY->setLabelsFont(QFont("Arial", 12));  // 设置X轴的字体为Arial，大小为10

    startDataInsertion(axisX, series, chart, chartView);  // 传入chartView，确保更新
    return chartView;
}

void MainWindow::startDataInsertion(QDateTimeAxis *axisX, QLineSeries *series, QChart *chart1, QChartView *chartView) {
    // 保存参数供定时器使用
    axisX1 = axisX;
    series1 = series;
    chart = chart1;
    chartView1 = chartView;

    // 如果定时器尚未启动，则启动定时器
    if (Timer1 == -1) {
        // 启动定时器，每秒触发一次
//        Timer1 = startTimer(1000);  // 启动定时器，1000毫秒（1秒）
        qDebug() << "定时器已启动!";
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
    axisX->setLabelsFont(QFont("Arial", 12));  // 设置X轴的字体为Arial，大小为10
    axisY->setLabelsFont(QFont("Arial", 12));  // 设置X轴的字体为Arial，大小为10
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
//        qDebug() << "定时器2已启动!";

    }
}

// 定时器触发事件
void MainWindow::timerEvent(QTimerEvent *event) {
    // 判断是否是 Timer2 定时器
    if (event->timerId() == Timer2) {
        qDebug() << "定时器2触发: 更新数据" << "id==" << event->timerId() << endl; ;

        if (series2 && axisX2 && chart02 && chartView2) {
            qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();  // 获取当前时间戳（毫秒）

            // 将全局变量 a 插入到曲线中，currentTime 作为 X 值，a 作为 Y 值
            series2->append(currentTime, a);
            qDebug() << "Inserted data point: " << currentTime << ", " << a;
#if 1
            // 如果定时器开始将a的值存入容器中
            chart2Container.append(a);

#if 1
            // 检查是否已经存在相同的记录
            bool isUnique = true;
            QDateTime currentDateTime = QDateTime::currentDateTime();
            for (const QList<QVariant> &existingData : streedataList) {
                // 假设数据的第一个元素是压力值，第二个是时间，第三个是"leftData"
                if (existingData[0].toString() == a &&  // 如果压力值相同
                    existingData[1].toString() == currentDateTime.toString("yyyy-MM-dd HH:mm:ss") && // 时间相同
                    existingData[2].toString() == "rightData") {  // 左侧数据相同
                    isUnique = false;
                    break;
                }
            }

            if (isUnique) {
                // 如果数据是唯一的，插入数据
                QList<QVariant> data;
                data.append(a);  // 存储压力值
                data.append(currentDateTime.toString("yyyy-MM-dd HH:mm:ss"));  // 存储时间
                data.append("rightData");  // 存储左侧数据
                // 将这个列表存入外层的容器
                streedataList.append(data);
            } else {
                qDebug() << "Data already exists, skipping insertion.";
            }

#endif

#endif
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
    if(event->timerId() == Timer1) {
      //do Timer1_Fuc
        qDebug() << "定时器1 start" << endl;
                qDebug() << "定时器1触发: 更新数据" << "id==" << event->timerId() << endl;
        if (series1 && axisX1 && chart && chartView1) {
            qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();  // 获取当前时间戳（毫秒）

            // 将全局变量 a 插入到曲线中，currentTime 作为 X 值，a 作为 Y 值
            series1->append(currentTime, a);
#if 1
            // 检查是否已经存在相同的记录
            bool isUnique = true;
            QDateTime currentDateTime = QDateTime::currentDateTime();
            for (const QList<QVariant> &existingData : streedataList) {
                // 假设数据的第一个元素是压力值，第二个是时间，第三个是"leftData"
                if (existingData[0].toString() == a &&  // 如果压力值相同
                    existingData[1].toString() == currentDateTime.toString("yyyy-MM-dd HH:mm:ss") && // 时间相同
                    existingData[2].toString() == "leftData") {  // 左侧数据相同
                    isUnique = false;
                    break;
                }
            }

            if (isUnique) {
                // 如果数据是唯一的，插入数据
                QList<QVariant> data;
                data.append(a);  // 存储压力值
                data.append(currentDateTime.toString("yyyy-MM-dd HH:mm:ss"));  // 存储时间
                data.append("leftData");  // 存储左侧数据
                // 将这个列表存入外层的容器
                streedataList.append(data);
            } else {
                qDebug() << "Data already exists, skipping insertion.";
            }

#endif
            qDebug() << "Inserted data point: " << currentTime << ", " << a;
#if 1
            // 如果定时器开始将a的值存入容器中
            chart1Container.append(a);
            // 容器中存储的值tuBianSet->text();

#endif
            // 最多显示 4 个数据点，删除最早的点
            if (series1->count() > 4) {
                series1->remove(0);  // 删除最早的数据点
            }

            // 获取当前数据点的数量
            int pointCount = series1->count();
            qint64 firstPointTime = series1->points().first().x();  // 获取最早的数据点的 X 值
            qint64 lastPointTime = series1->points().last().x();    // 获取最新的数据点的 X 值

            // 动态更新 X 轴范围
            axisX1->setMin(QDateTime::fromMSecsSinceEpoch(firstPointTime));  // 设置最小时间为最早的数据点
            axisX1->setMax(QDateTime::fromMSecsSinceEpoch(lastPointTime));   // 设置最大时间为最新的数据点

            // 根据数据点数目动态设置 X 轴的刻度数
            axisX1->setTickCount(qMin(4, pointCount));  // 显示 2 到 4 个 X 轴时间点

            // 强制更新图表视图
            chart->update();
            chartView1->repaint();  // 强制重绘，更新图表视图
        }
    }

}
