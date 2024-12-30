#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sqliteaction.h"

QPushButton *createJiLu;
QString buttonStyle;
QList<QPair<QDateTime, int>> valueContainer;  // 用于存储每个时间点对应的 a 值


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

#if 1
    ui->setupUi(this);
    /*-------------------20241228---------------------*/
//    setFixedSize(1280,1024);
    /*-------------------20241228---------------------*/

    w1 = nullptr;//历史界面
    w2 = nullptr; //工艺标准界面
     setAttribute(Qt::WA_QuitOnClose, true);  // 主窗口关闭时退出应用程序
     setWindowIcon(QIcon(":/img/QmodBusImg.svg"));
     qInstallMessageHandler(qDebugLogInfo::customMessageHandler); //打印日志
     qDebug("This is a debug message");
    // 设置主窗口
            setWindowTitle("悬挂件压装力测试系统");
            resize(1200, 800);
            MainWindow::a = 0;

            QFile file(":/mainForm.qss");
            file.open(QFile::ReadOnly);
            QString qss=file.readAll();
            file.close();
            applyStyles(this,qss);
            MainWindow::dataBaseConn = SqliteAction::getDatabaseConnection("./D1.db");
            MainWindow::initializeControls();
            MainWindow::initBuJu();
#endif
// 设置日期显示格式为 "yyyy-MM-dd"
yaZhuangData->setDisplayFormat("yyyy-MM-dd");
// 设置日期为当前日期
yaZhuangData->setDate(QDate::currentDate());

tuBianSet->setText("80");

//QChart *chart = new QChart();
/**操作通过plc获取压装力值绘制到曲线上**/
    timer->start(1000);
    elapsedTimer->start();
#if 1
    /**
     * @brief 定时器槽函数 用于处理实时显示plc的值
     */
    connect(timer, &QTimer::timeout, this, [=] {
        startRefun1();
        zhanKaiLine->setText(QString::number(a)); // 将 int 转换为 QString

        //定义一个容器将a的值存入容器只保存60个值 每满60 将旧数据删除一个再添加一个
        //按照成对保存 a的值 还有存入容器的当前时间时间格式年月日时分秒
        // 获取当前时间（年月日时分秒格式）
       QDateTime currentTime = QDateTime::currentDateTime();

       // 将当前时间和 a 的值一起存入容器
       valueContainer.append(qMakePair(currentTime, a));

       // 如果容器的大小超过 60，删除最旧的一个数据点
       if (valueContainer.size() > 60) {
           valueContainer.removeFirst();
       }

    });
#endif

#if 1  
    /**
     * @brief 处理数据库与控件同步
     * 操作者同步
     */
                /***********************bash-20241212*******************/
    bool flags = SqliteAction::queryAllDataFromTable(dataBaseConn,"operatorTb", dataList);
    if(flags){
        qDebug() << "查询 operatorTb 成功!!!";

        // 清空 QComboBox，准备添加新的项
        caoZuoName->clear();  // 清空 QComboBox 中的所有项
        // 遍历 dataList 将每一行的 xuanName 列添加到 QComboBox 中
        for (const QList<QVariant>& row : dataList) {
            if (!row.isEmpty()) {
                // 假设 xuanName 是 dataList 每行的第一列（根据你的表结构调整列索引）
                QString xuanName = row.at(1).toString();  // 获取 xuanName 的值
                caoZuoName->addItem(xuanName);  // 将该值添加到 QComboBox
            }
        }
    }
                /***********************bash-20241212*******************/

                /***********************bash-20241212*******************/
    //// 检查者同步
    flags = SqliteAction::queryAllDataFromTable(dataBaseConn,"inspectorTb", dataList1);
    if(flags){
        qDebug() << "查询 inspectorTb 成功!!!";

        // 清空 QComboBox，准备添加新的项
        jianChaName->clear();  // 清空 QComboBox 中的所有项
        // 遍历 dataList 将每一行的 xuanName 列添加到 QComboBox 中
        for (const QList<QVariant>& row : dataList1) {
            if (!row.isEmpty()) {
                // 假设 xuanName 是 dataList 每行的第一列（根据你的表结构调整列索引）
                QString xuanName = row.at(1).toString();  // 获取 xuanName 的值
                jianChaName->addItem(xuanName);  // 将该值添加到 QComboBox
            }
        }
    }
             /***********************bash-20241212*******************/

            /***********************bash-20241211*******************/
    //// 悬挂件标准同步
    flags = SqliteAction::queryAllDataFromTableXuan(dataBaseConn, "proStds", dataList2);
        if(flags){
            qDebug() << "查询 inspectorTb 成功!!!";

            // 清空 QComboBox，准备添加新的项
            xuanGuaName->clear();  // 清空 QComboBox 中的所有项
            // 遍历 dataList 将每一行的 xuanName 列添加到 QComboBox 中
            for (const QList<QVariant>& row : dataList2) {
                if (!row.isEmpty()) {
                    // 假设 xuanName 是 dataList 每行的第一列（根据你的表结构调整列索引）
                    QString xuanName = row.at(1).toString();  // 获取 xuanName 的值
                    xuanGuaName->addItem(xuanName);  // 将该值添加到 QComboBox
                }
            }
        }
           /***********************bash-20241211*******************/
#endif

#if 1
    /**
     * @brief 连接 QComboBox 的 currentIndexChanged 信号到槽函数
     * 单击悬挂名称时会自动将标准显示到标准框里
     */
    connect(xuanGuaName, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        MainWindow::clearChart(chartView1);
        MainWindow::clearChart(chartView2);
        /***************bash20241213*************/
#if 1
        MainWindow::configureChart(chartView1, chart,series1, axisX1, axisY1, "压力曲线1");
        MainWindow::configureChart(chartView2, chart02,series2, axisX2, axisY2, "压力曲线2");
#endif
        /***************bash20241213*************/
        yaZhuang1->setText("");
        yaZhuangSaultLine1->setText("");
        yaZhuang2->setText("");
        yaZhuangSaultLine2->setText("");
        /***********************bash-20241211*******************/
        // 如果索引有效
        if (index >= 0 && index < dataList2.size()) {

            // 获取当前选中的项对应的数据行
            const QList<QVariant>& selectedRow = dataList2.at(index);
            // 打印数据库中对应的主键id
            qDebug() << "单击悬挂名称索引:" << selectedRow.at(0).toString() << endl;

            // 假设 selectedRow 的结构是：[xuanName, pressStd1, pressStd2]
            if (selectedRow.size() >= 3) {
                // 获取对应的压力标准
                QString pressStd1 = selectedRow.at(2).toString();  // 获取 pressStd1
                QString pressStd2 = selectedRow.at(2).toString();  // 获取 pressStd2

                // 设置压力标准文本
                yaZhuangStdLine1->setText(pressStd1);
                yaZhuangStdLine2->setText(pressStd2);
            }
        }
        /***********************bash-20241211*******************/

    });
#endif

#if 1
    /**
     * @brief 点击创建连接数据库，将主记录信息插入数据库
     */
    connect(wanCheng, QPushButton::clicked,this, [=]{
        // 创建消息框
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认", "是否确认执行操作?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes){
           qDebug() << "创建记录" << endl;

            bool result1 = SqliteAction::ensureTableInDatabase(dataBaseConn, "./D1.db", "mainListTb");
            if(result1)
                qDebug() << "Table创建成功!!!"<< endl;
            QList<QVariant> values;

            // 将各个控件的数据存入 QList<QVariant>
            values.append(xuanGuaName->currentText());       // 选挂名称
            values.append(yaZhuangData->text()+" "+ (QTime::currentTime()).toString());// 压装日期
            //values.append(yaZhuangData->text());             // 压装日期
            values.append(caoZuoName->currentText());        // 操作人
            values.append(jianChaName->currentText());       // 检查人

            // 第一组压装数据
            values.append(jieDianSignLine1->text());         // 接点编号1
            values.append(yaZhuang1->text());                // 压装值1
            values.append(yaZhuangSaultLine1->text());       // 压装结果1
            values.append(yaZhuangStdLine1->text());         // 压装标准1

            // 第二组压装数据
            values.append(jieDianSignLine2->text());         // 接点编号2
            values.append(yaZhuang2->text());                // 压装值2
            values.append(yaZhuangSaultLine2->text());       // 压装结果2
            values.append(yaZhuangStdLine2->text());         // 压装标准2

            // 遍历并输出存储的数据，方便调试
            qDebug() << "Stored Values:";
            for (const QVariant &value : values) {
                qDebug() << value;
            }
            QList<QString> columns = {
                "xuanName", "press_date", "operator", "inspector",
                "serial_number1", "pressData1", "press_result1", "force_standard1",
                "serial_number2", "pressData2", "press_result2", "force_standard2"
            };

            bool result2 = SqliteAction::insertIntoTable(dataBaseConn, "./D1.db", "mainListTb", columns, values);

            if(result2)
                qDebug() << "数据插入成功!!!"<< endl;

            // 点击创建获取当前插入主记录中的id
            // streedataList存储了曲线1和2 的压力值曲线时间 标志位
            // 创建表
            bool flags = SqliteAction::streetTableInDatabase(dataBaseConn, "./D1.db", "streetDataTb");
            if(flags)
                qDebug() << "创建streeDataTb成功" << endl;
            // 查询表mainListTb 将最后一条记录的主键值转QString返回
            QString _mainId = SqliteAction::getLastRecordId(dataBaseConn, "mainListTb");
            // 将streedataList容器和主记录id存入数据库中
            flags = SqliteAction::insertStreetData(dataBaseConn, "streetDataTb", streedataList, _mainId);
                    if(flags)
                        qDebug() << "数据插入streeDataTb成功" << endl;
            MainWindow::clearChart(chartView1);
            MainWindow::clearChart(chartView2);
            MainWindow::configureChart(chartView1, chart,series1, axisX1, axisY1, "压力曲线1");
            MainWindow::configureChart(chartView2, chart02,series2, axisX2, axisY2, "压力曲线2");

        }else{
            qDebug() << "用户选择了否，返回";
                 return;  // 结束函数，什么都不做
        }
    });
#endif

#if 1
    /**
     * @brief 页面跳转到历史界面
     */
    connect(recordQueryButton, &QPushButton::clicked, this, [=] {
        qDebug() << "即将跳转历史页面......";
        // 在新窗体中显示查询结果
        w1 = new HstoryList(this);
        w1->show();
        w1->setWindowModality(Qt::ApplicationModal);
//        MainWindow::w1.show();

#if 1 // 处理历史界面点击table行时 重写主界面数据
        connect(w1, &HstoryList::dataUpdated, this, [=](QList<QVariant> mainData, QList<QVariant> chartsData){
//            for (const QVariant& data : mainData) {
//                qDebug() << "main data：" <<data.toString();
//            }
//            for (const QVariant& data1 : chartsData) {
//                qDebug() << "charts data：" << data1.toString();
//            }
            // 处理从另一个界面拿到的数据ui到当前界面
            // 1)悬挂名称
            QString targetValue = mainData[0].toString();  // 获取要比较的值
            // 遍历 QComboBox 中的所有项
            for (int i = 0; i < xuanGuaName->count(); ++i) {
                if (xuanGuaName->itemText(i) == targetValue) {
                    // 如果当前项与目标值匹配
                    xuanGuaName->setCurrentIndex(i);  // 设置当前选中项
                    break;  // 找到匹配项后退出循环
                }
            }
            // 2)压装时期
            QString dateTime = mainData[1].toString();  // 获取原始字符串 "2024-12-04 14:59:04"
            QStringList parts = dateTime.split(" ");  // 以空格分割日期和时间
            QString datePart = parts.at(0);  // 获取分割后的日期部分 "2024-12-04"
            //yaZhuangData  是 QDataEdit类型 我需要把切割后的dateTime赋值给yaZhuangData
            QDate date = QDate::fromString(datePart, "yyyy-MM-dd");
            yaZhuangData->setDate(date);
            // 3)操作者
            QString targetValue2 = mainData[2].toString();  // 获取要比较的值
            // 遍历 QComboBox 中的所有项
            for (int i = 0; i < caoZuoName->count(); ++i) {
                if (caoZuoName->itemText(i) == targetValue2) {
                    // 如果当前项与目标值匹配
                    caoZuoName->setCurrentIndex(i);  // 设置当前选中项
                    break;  // 找到匹配项后退出循环
                }
            }
            // 4)检查者
            QString targetValue3 = mainData[3].toString();  // 获取要比较的值
            // 遍历 QComboBox 中的所有项
            for (int i = 0; i < jianChaName->count(); ++i) {
                if (jianChaName->itemText(i) == targetValue3) {
                    // 如果当前项与目标值匹配
                    jianChaName->setCurrentIndex(i);  // 设置当前选中项
                    break;  // 找到匹配项后退出循环
                }
            }
            // 5)节点序列号1
            jieDianSignLine1->setText(mainData[4].toString());
            // 6)压装力值1
            yaZhuang1->setText(mainData[5].toString());
            // 7)压装结果1
            yaZhuangSaultLine1->setText(mainData[6].toString());
            // 8)压装标准1因为和xuanGuaNameQComboBox关联了可以省略
            // 9)节点序列号2
            jieDianSignLine2->setText(mainData[8].toString());
            // 10)压装力值2
            yaZhuang2->setText(mainData[9].toString());
            // 11)压装结果2
            yaZhuangSaultLine2->setText(mainData[10].toString());
            // 12)压装标准2也和xuanGuaNameQComboBox关联了可以省略

            // 上面是主记录处理下面是曲线处理
            // chartsData 是QList<QVariant>类型
            // 数据拆分成 左表数据 和右表数据
            // leftData和rightData 前的两个值是一对存成容器中
            // 左表和右表的数据容器
            // 使用 QMap 来存储时间（包含时分秒）和多个压力值的键值对
                MainWindow::processChartsData(chartsData);
                // 绘制曲线
#if 1
                // 判断左边的数量如果小于等于4直接操作  如果大于四则之前第一个的数据不要后面的要一个这样递推
                // 右侧判断也一样
                // 处理左表数据

                // 左边数据处理
                selectedData.clear();
                for (auto it = leftData.begin(); it != leftData.end(); ++it) {
                    QList<QString> pressureValues = it.value();
                    QString dataValues = it.key();

                    if (pressureValues.size() <= 4) {

                        for (int i = 0; i < pressureValues.size(); ++i) {
                            QList<QString> dataPair;
                            dataPair.append(dataValues);
                            dataPair.append(pressureValues[i]);
                            selectedData.append(dataPair);
//                            qDebug() << "Time: " << dataPair[0] << " Pressure: " << dataPair[1];
                        }

                    } else {

                        for (int i = pressureValues.size() - 4; i >= 0; --i) {
                            for (int j = i; j < i + 4; ++j) {
                                QList<QString> dataPair;
                                dataPair.append(dataValues);
                                dataPair.append(pressureValues[j]);  // Add corresponding pressure value
                                selectedData.append(dataPair); // Store the pair in selectedData
                            }
                        }
                    }
                }
                for (const auto &pair : selectedData) {
                    qDebug() << "Time: " << pair[0] << " Pressure: " << pair[1];
                }

                createChartView123(chartView1, selectedData, "压力曲线1", series1, chart, axisX1, axisY1);
#if 1
                // 右边数据处理
                selectedData.clear();
                for (auto it = rightData.begin(); it != rightData.end(); ++it) {
                    QList<QString> pressureValues = it.value();
                    QString dataValues = it.key();

                    if (pressureValues.size() <= 4) {

                        for (int i = 0; i < pressureValues.size(); ++i) {
                            QList<QString> dataPair;
                            dataPair.append(dataValues);
                            dataPair.append(pressureValues[i]);
                            selectedData.append(dataPair);
//                            qDebug() << "Time: " << dataPair[0] << " Pressure: " << dataPair[1];
                        }

                    } else {

                        for (int i = pressureValues.size() - 4; i >= 0; --i) {
                            for (int j = i; j < i + 4; ++j) {
                                QList<QString> dataPair;
                                dataPair.append(dataValues);
                                dataPair.append(pressureValues[j]);  // Add corresponding pressure value
                                selectedData.append(dataPair); // Store the pair in selectedData
                            }
                        }
                    }
                }
                for (const auto &pair : selectedData) {
                    qDebug() << "Time: " << pair[0] << " Pressure: " << pair[1];
                }
                createChartView123(chartView2, selectedData, "压力曲线1", series2, chart02, axisX2, axisY2);
#endif

#endif
        });
#endif

#if 0 // 处理历史界面点击table行时 重写主界面数据 优化版
    connect(w1, &HstoryList::dataUpdated, this, [=](QList<QVariant> mainData, QList<QVariant> chartsData){
        // 处理从另一个界面拿到的数据ui到当前界面

        // 1)悬挂名称
        QString targetValue = mainData[0].toString();  // 获取要比较的值
        for (int i = 0; i < xuanGuaName->count(); ++i) {
            if (xuanGuaName->itemText(i) == targetValue) {
                xuanGuaName->setCurrentIndex(i);  // 设置当前选中项
                break;  // 找到匹配项后退出循环
            }
        }

        // 2)压装时期
        QString dateTime = mainData[1].toString();  // 获取原始字符串 "2024-12-04 14:59:04"
        QStringList parts = dateTime.split(" ");  // 以空格分割日期和时间
        QString datePart = parts.at(0);  // 获取分割后的日期部分 "2024-12-04"
        QDate date = QDate::fromString(datePart, "yyyy-MM-dd");
        yaZhuangData->setDate(date);  // 设置日期

        // 3)操作者
        QString targetValue2 = mainData[2].toString();  // 获取要比较的值
        for (int i = 0; i < caoZuoName->count(); ++i) {
            if (caoZuoName->itemText(i) == targetValue2) {
                caoZuoName->setCurrentIndex(i);  // 设置当前选中项
                break;  // 找到匹配项后退出循环
            }
        }

        // 4)检查者
        QString targetValue3 = mainData[3].toString();  // 获取要比较的值
        for (int i = 0; i < jianChaName->count(); ++i) {
            if (jianChaName->itemText(i) == targetValue3) {
                jianChaName->setCurrentIndex(i);  // 设置当前选中项
                break;  // 找到匹配项后退出循环
            }
        }

        // 5)节点序列号1
        jieDianSignLine1->setText(mainData[4].toString());

        // 6)压装力值1
        yaZhuang1->setText(mainData[5].toString());

        // 7)压装结果1
        yaZhuangSaultLine1->setText(mainData[6].toString());

        // 9)节点序列号2
        jieDianSignLine2->setText(mainData[8].toString());

        // 10)压装力值2
        yaZhuang2->setText(mainData[9].toString());

        // 11)压装结果2
        yaZhuangSaultLine2->setText(mainData[10].toString());

        // 上面是主记录处理，下面是曲线处理
        // chartsData 是QList<QVariant>类型
        // 数据拆分成 左表数据 和右表数据
        MainWindow::processChartsData(chartsData);  // 处理曲线数据

        // 处理左边数据
        selectedData.clear();
        for (auto it = leftData.begin(); it != leftData.end(); ++it) {
            QList<QString> pressureValues = it.value();
            QString dataValues = it.key();

            if (pressureValues.size() <= 4) {
                // 如果小于等于4，直接添加
                for (int i = 0; i < pressureValues.size(); ++i) {
                    QList<QString> dataPair = { dataValues, pressureValues[i] };
                    selectedData.append(dataPair);
                }
            } else {
                // 如果大于4，只取最近4个数据
                for (int i = pressureValues.size() - 4; i >= 0; --i) {
                    for (int j = i; j < i + 4; ++j) {
                        QList<QString> dataPair = { dataValues, pressureValues[j] };
                        selectedData.append(dataPair);
                    }
                }
            }
        }
        // 调试输出，可以根据需要取消注释
        // for (const auto& pair : selectedData) {
        //     qDebug() << "Time: " << pair[0] << " Pressure: " << pair[1];
        // }
        createChartView123(chartView1, selectedData, "压力曲线1", series1, chart, axisX1, axisY1);

        // 处理右边数据
        selectedData.clear();  // 清空左侧处理的选中数据
        for (auto it = rightData.begin(); it != rightData.end(); ++it) {
            QList<QString> pressureValues = it.value();
            QString dataValues = it.key();

            if (pressureValues.size() <= 4) {
                // 如果小于等于4，直接添加
                for (int i = 0; i < pressureValues.size(); ++i) {
                    QList<QString> dataPair = { dataValues, pressureValues[i] };
                    selectedData.append(dataPair);
                }
            } else {
                // 如果大于4，只取最近4个数据
                for (int i = pressureValues.size() - 4; i >= 0; --i) {
                    for (int j = i; j < i + 4; ++j) {
                        QList<QString> dataPair = { dataValues, pressureValues[j] };
                        selectedData.append(dataPair);
                    }
                }
            }
        }
        // 调试输出，可以根据需要取消注释
        // for (const auto& pair : selectedData) {
        //     qDebug() << "Time: " << pair[0] << " Pressure: " << pair[1];
        // }
        createChartView123(chartView2, selectedData, "压力曲线2", series2, chart02, axisX2, axisY2);
    });
#endif


    });
#endif

#if 1
    /**
     * @brief 页面跳转到设置界面
     */
    connect(standardButton, QPushButton::clicked,this,[=]{
       qDebug() << "即将跳转工艺标准界面" << endl;
       w2 = new ConfigSet(this);
       w2->setWindowModality(Qt::ApplicationModal);
       /***********************bash-20241210*******************/
       connect(w2, &ConfigSet::sendDataBToCWidget, this, &MainWindow::onReceiveDataFromBWidget);
       /***********************bash-20241210*******************/

       /***********************bash-20241212*******************/
       connect(w2, &ConfigSet::sendDataBToCWidget2, this, &MainWindow::onReceiveDataFromBWidget2);
       /***********************bash-20241212*******************/
       MainWindow::w2->show();
    });
#endif

#if 1


#if 1
    /**
     * @brief 绘制图表1
     */
connect(startReBtn1, &QPushButton::clicked, this, [=]() {
        c = 0;
        if(jieDianSignLine1->text() == ""){
            QMessageBox::warning(this, "开始失败", "节点序列号1不能为空！");
            return;
        }
        if(yaZhuangStdLine1->text() == ""){
            QMessageBox::warning(this, "开始失败", "请先选择悬挂名称！");
            return;
        }
            // 点击开始1清空 压力数据容器
            streedataList.clear();
            MainWindow::stopTimers();
            Timer1 = startTimer(1000);
            MainWindow::clearChart(chartView1);
            chartView1 = createChartView("压力曲线1", axisX1, axisY1);
            chartLayout->addWidget(chartView1, 1, 0, 1, 3);  // Chart 1: Spans 2 columns (Column 0-1)
            yaZhuang1->setText("");
            yaZhuangSaultLine1->setText("");
            // 处理容器中的值插入到数据库中和主记录表关联
            chart1Container.clear();
//            series1->setColor(Qt::red);

//            startReBtn1->setEnabled(false);  // 确保按钮是启用的
//            startReBtn1->setStyleSheet(
//                "QPushButton { background-color: #E9EBFE; color: black; }"  // 启用时的背景色和文字颜色
//                "QPushButton:disabled { background-color: rgba(233, 235, 254, 5); color: black; }"  // 禁用时透明背景，文字不变
//            );



});
#endif

#if 1
    /**
     * @brief 处理定时器停止后图标1操作
     */
connect(jieShu1, &QPushButton::clicked, this, [=]() {

    // 这里进行判断 和突变值进行比较 如果大于突变值则将大于突变值的值和突变值前三个数
    // 在图表中显示
    killTimer(Timer1);

    // 解析 yaZhuangStdLine1->text() 的范围，假设格式是 "min~max"
    QString rangeText = yaZhuangStdLine1->text(); // 获取范围文本
    QStringList rangeParts = rangeText.split("~"); // 使用 ~ 分割成两个部分

    if (rangeParts.size() == 2) {
        /*-----------------20241228--------------------*/
        // 将 min 和 max 转换为整数
          float minRange = rangeParts[0].toFloat();
          float maxRange = rangeParts[1].toFloat();

        /*-----------------20241228--------------------*/
        // 遍历 chart1Container 容器中的每个值
        int lastValidValue = -1;  // 用于存储最后一个符合范围的值

        for (int value : chart1Container) {
            // 如果值在范围内
            if (value >= minRange && value <= maxRange) {
                lastValidValue = value;  // 更新最后一个符合范围的值
            }

        }

        // 如果找到符合条件的值
        if (lastValidValue != -1) {
            yaZhuangSaultLine1->setText("合格");  // 设置合格状态
            yaZhuang1->setText(QString::number(lastValidValue));  // 显示最后一个符合条件的值
        } else {
            yaZhuangSaultLine1->setText("不合格");  // 如果没有找到符合条件的值
            yaZhuang1->setText("无数据");
        }
    } else {
        qDebug() << "Range format error! Expected format: min~max";
    }
    qDebug() << "定时器1 close"<< Timer1 << endl;
});
#endif

#if 1
    /**
     * @brief 绘制图表2
     */
connect(startReBtn2, &QPushButton::clicked,this,[=]{
            if(jieDianSignLine2->text() == ""){
                QMessageBox::warning(this, "开始失败", "节点序列号2不能为空！");
                return;
            }
            if(yaZhuangStdLine2->text() == ""){
                QMessageBox::warning(this, "开始失败", "请先选择悬挂名称！");
                return;
            }

            MainWindow::stopTimers();
            Timer2 = startTimer(1000);  // 每秒插入一次数据
            MainWindow::clearChart(chartView2);
            chartView2 = createChartView2("压力曲线2", axisX2, axisY2);
            chartLayout->addWidget(chartView2, 1, 3, 1, 3);  // Chart 2: Spans 3 columns (Column 2-4)
            yaZhuang2->setText("");
            yaZhuangSaultLine2->setText("");
            chart2Container.clear();
//            series2->setColor(Qt::red);
});
#endif

#if 1
    /**
     * @brief 处理定时器停止后图标2操作
     */
connect(jieShu2, &QPushButton::clicked,this,[=]{
            killTimer(Timer2);

            // 解析 yaZhuangStdLine1->text() 的范围，假设格式是 "min~max"
            QString rangeText = yaZhuangStdLine2->text(); // 获取范围文本
            QStringList rangeParts = rangeText.split("~"); // 使用 ~ 分割成两个部分

            if (rangeParts.size() == 2) {
                // 将 min 和 max 转换为整数
//                int minRange = rangeParts[0].toInt();
//                int maxRange = rangeParts[1].toInt();
              /*-----------------20241228--------------------*/
                float minRange = rangeParts[0].toFloat();
                float maxRange = rangeParts[1].toFloat();

              /*-----------------20241228--------------------*/
                // 遍历 chart2Container 容器中的每个值
                int lastValidValue = -1;  // 用于存储最后一个符合范围的值

                for (int value : chart2Container) {
                    // 如果值在范围内
                    if (value >= minRange && value <= maxRange) {
                        lastValidValue = value;  // 更新最后一个符合范围的值
                    }

                }

                // 如果找到符合条件的值
                if (lastValidValue != -1) {
                    yaZhuangSaultLine2->setText("合格");  // 设置合格状态
                    yaZhuang2->setText(QString::number(lastValidValue));  // 显示最后一个符合条件的值
                } else {
                    yaZhuangSaultLine2->setText("不合格");  // 如果没有找到符合条件的值
                    yaZhuang2->setText("无数据");
                }
            } else {
                qDebug() << "Range format error! Expected format: min~max";
            }
            qDebug() << "close" << endl;


            for (const QList<QVariant>& dataList : streedataList) {
                for (const QVariant& var : dataList) {
                    qDebug() << "Data:" << var.toString();
                }
            }


});
#endif

#if 1
    /**
     * @brief 保存图表1成图片
     */
connect(daYinChartBtn1, QPushButton::clicked,this, [=]{

//    QSize newSize1(450, 400);  // 新的图像大小
//    QSize newSize2(450, 400);  // 新的图像大小
    MainWindow::saveChartToImage(chartView1, "./chart1.png");
    MainWindow::saveChartToImage(chartView2, "./chart2.png");
        /***************bash20241213*************/
    MainWindow::exportPdf();
        /***************bash20241213*************/

        /***************bash20241227*************/
    MainWindow::backupChartFile();
        /***************bash20241227*************/
});
#endif

#if 1
    /**
     * @brief 保存图表2成图片
     */
connect(daYinChartBtn1, QPushButton::clicked,this, [=]{

});
#endif




//fengMingQi->hide();
#if 1
connect(fengMingQi, QPushButton::clicked, this, [=]{

     MainWindow::startRefun2();
     b = dex;
     if(b == 1){
         MainWindow::on_writeTor();
         qDebug() << "蜂鸣器已被关闭" << b << endl;
         dex = 0;
         return;
     }else if(b == 0){
         MainWindow::on_writeTor();
         dex = 1;
         qDebug() << "蜂鸣器已被开启" << b << endl;
         return;
     }else{
         qDebug() << b << ":" << endl;
         return;
     }
});
#endif


// 数据点高亮显示在曲线上
connect(shuJuBox, &QCheckBox::toggled, this, &MainWindow::onCheckBoxToggled);


// 状态栏
statusBar = new QStatusBar(this);
setStatusBar(statusBar);

// 设置状态栏样式
statusBar->setStyleSheet("QStatusBar { background-color: #34495e; color: white; font-size: 12px; }");
#endif
}

void MainWindow::onReceiveDataFromBWidget(const int &data) {
    qDebug() << "Received data 11111in ConfigSet:" << data;
    /***********************bash-20241211*******************/
//// 通过信号悬挂件标准同步
bool flags = SqliteAction::queryAllDataFromTableXuan(dataBaseConn, "proStds", dataList2);
if(flags){
    qDebug() << "查询 inspectorTb 成功!!!";

    // 清空 QComboBox，准备添加新的项
    xuanGuaName->clear();  // 清空 QComboBox 中的所有项
    // 遍历 dataList 将每一行的 xuanName 列添加到 QComboBox 中
    for (const QList<QVariant>& row : dataList2) {
        if (!row.isEmpty()) {
            // 假设 xuanName 是 dataList 每行的第一列（根据你的表结构调整列索引）
            QString xuanName = row.at(1).toString();  // 获取 xuanName 的值
            xuanGuaName->addItem(xuanName);  // 将该值添加到 QComboBox
        }
    }
}
   /***********************bash-20241211*******************/
}

void MainWindow::onReceiveDataFromBWidget2(const int &data) {
    qDebug() << "接到UserCreate界面信号:" << data << endl;
    /***********************bash-20241212*******************/
bool flags = SqliteAction::queryAllDataFromTable(dataBaseConn,"operatorTb", dataList);
if(flags){
qDebug() << "查询 operatorTb 成功!!!";

// 清空 QComboBox，准备添加新的项
caoZuoName->clear();  // 清空 QComboBox 中的所有项
// 遍历 dataList 将每一行的 xuanName 列添加到 QComboBox 中
    for (const QList<QVariant>& row : dataList) {
        if (!row.isEmpty()) {
            // 假设 xuanName 是 dataList 每行的第一列（根据你的表结构调整列索引）
            QString xuanName = row.at(1).toString();  // 获取 xuanName 的值
            caoZuoName->addItem(xuanName);  // 将该值添加到 QComboBox
        }
    }
}
    /***********************bash-20241212*******************/

    /***********************bash-20241212*******************/
//// 检查者同步
flags = SqliteAction::queryAllDataFromTable(dataBaseConn,"inspectorTb", dataList1);
if(flags){
qDebug() << "查询 inspectorTb 成功!!!";

// 清空 QComboBox，准备添加新的项
jianChaName->clear();  // 清空 QComboBox 中的所有项
// 遍历 dataList 将每一行的 xuanName 列添加到 QComboBox 中
    for (const QList<QVariant>& row : dataList1) {
        if (!row.isEmpty()) {
            // 假设 xuanName 是 dataList 每行的第一列（根据你的表结构调整列索引）
            QString xuanName = row.at(1).toString();  // 获取 xuanName 的值
            jianChaName->addItem(xuanName);  // 将该值添加到 QComboBox
        }
    }
}
 /***********************bash-20241212*******************/
}


MainWindow::~MainWindow()
{
    // 在销毁窗口时关闭数据库连接
       if (dataBaseConn.isOpen()) {
           dataBaseConn.close();
           qDebug() << "Database connection closed!!!.";
       }
       if(w1) delete w1;
       if(w2) delete w2;
    delete ui;
//    MainWindow::w1.close();
}


/**
 * @brief 初始化控件
 */
void MainWindow::initializeControls()
{
    timer = new QTimer();
    elapsedTimer = new QElapsedTimer(); //记录时间
    startReBtn1 = new QPushButton("开始接收", this);
    startReBtn2 = new QPushButton("开始接收", this);

    modbusDevice = new QModbusTcpClient(this);

    caoZuoName = new QComboBox(this);
    yaZhuangData = new QDateEdit(this);
    axisX1 = new QDateTimeAxis(this);
    axisY1 = new QValueAxis(this);
    axisX2 = new QDateTimeAxis(this);
    axisY2 = new QValueAxis(this);
    chart02 = new QChart();
    chart = new QChart();

    // 创建两个图表并初始化
     chartView1 = new QChartView();
     chartView2 = new QChartView();

     // 创建控件
     xuanGuaName = new QComboBox(this);     // 悬挂件名称
     xiuZhengLine = new QLineEdit(this);    // 修正系数输入框
     xiuZhengBtn = new QPushButton("修正曲线", this);  // 修正按钮
     xiuZhengLabel = new QLabel("修正系数：", this);
     yaZhuangData = new QDateEdit(this);  // 创建 QDateEdit 控件

     saoMiaoData = new QSpinBox(this);       // 扫描时间

     saveBtn = new QPushButton("保存", this);  // 保存按钮
     caoZuoName = new QComboBox(this);      // 操作者下拉框

     tuBianSet = new QLineEdit(this);       // 突变跨度设置

     saveTuBianBtn = new QPushButton("保存", this);  // 保存突变按钮
     jianChaName = new QComboBox(this);     // 检查者下拉框
     xuanGuaName = new QComboBox(this);

     // 左侧控件组
     jieDianSignLine1 = new QLineEdit(this);
     daYinChartBtn1 = new QPushButton("打印图表", this);
     yaZhuang1 = new QLineEdit(this);
     // 左侧第二行
     yaZhuangSaultLine1 = new QLineEdit(this);
     yaZhuangStdLine1 = new QLineEdit(this);


     // 右侧控件组（与左侧对称）
     jieDianSignLine2 = new QLineEdit(this);
     daYinChartBtn2 = new QPushButton("打印图表", this);
     yaZhuang2 = new QLineEdit(this);
     // 右侧第二行
     yaZhuangSaultLine2 = new QLineEdit(this);
     yaZhuangStdLine2 = new QLineEdit(this);
     // 创建按钮并设置大小策略
     loginButton = new QPushButton("登录", this);
     logoutButton = new QPushButton("注销", this);
     standardButton = new QPushButton("悬挂件/压装力标准值和范围", this);
     recordQueryButton = new QPushButton("记录查询", this);
     dataMaintenanceButton = new QPushButton("数据维护", this);
     exitButton = new QPushButton("退出", this);
     zhanKaiLine = new QLineEdit("0", this);  // 展开值
     zhanKaiLine->setReadOnly(true); // 只读
     zhanKaiLine->setStyleSheet("QLineEdit {"
                                 "font-size: 30pt; "  // 设置字体大小
                                 "font-family: Arial; "  // 设置字体类型
                                 "text-align: center; "  // 设置文本居中
                                 "border: 2px solid black; "  // 设置边框
                                 "background-color: lightgray; "  // 设置背景颜色
                                 "}");
     zhanKaiLine->setAlignment(Qt::AlignCenter);
     zhanKaiLine->setFixedWidth(200);

     zhanKaiBtn = new QPushButton("展开", this);  // 展开按钮
     qieHuan = new QLabel("主设备:0\n副设备:0\nF5切换:主设备\n数据来源:仪表",this);

     series1 = new QLineSeries();
     series2 = new QLineSeries();

     createJiLu = new QPushButton("创建", this);  // 创建记录按钮
}

/**
 * @brief 控件布局
 */
void MainWindow::initBuJu(){
    // 创建菜单栏
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    menuBar->addMenu(" ");
//    menuBar->addMenu("设置(&Y)");
//    menuBar->addMenu("维护(&P)");
//    menuBar->addMenu("工具(&T)");
//    menuBar->addMenu("帮助(&Z)");

    // 设置菜单栏样式
    menuBar->setStyleSheet("QMenuBar { background-color: #2c3e50; color: white; font-size: 16px; }"
                           "QMenuBar::item { background-color: #34495e; padding: 5px 10px; }"
                           "QMenuBar::item:selected { background-color: #1abc9c; }");

    // 创建主窗口的中心部件
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    // 主布局
    mainLayout = new QGridLayout(centralWidget);

    // 创建垂直布局 (QVBoxLayout)
    QGridLayout *verticalLayout = new QGridLayout();
    // 添加顶部按钮组
    QHBoxLayout *topButtonLayout = new QHBoxLayout();
    topButtonLayout->addStretch(1); // Add some space at the beginning to push the buttons rightward

    // 设置按钮大小策略
    loginButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    logoutButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    standardButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    recordQueryButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    dataMaintenanceButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    exitButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // 添加按钮到布局
    topButtonLayout->addWidget(loginButton);
    topButtonLayout->addWidget(logoutButton);
    topButtonLayout->addWidget(standardButton);
    topButtonLayout->addWidget(recordQueryButton);
    topButtonLayout->addWidget(dataMaintenanceButton);
    topButtonLayout->addWidget(exitButton);

    verticalLayout->addWidget(loginButton,0,0);
    verticalLayout->addWidget(logoutButton,0,1);
    verticalLayout->addWidget(standardButton,0,2);
    verticalLayout->addWidget(recordQueryButton,0,3);
    verticalLayout->addWidget(dataMaintenanceButton,0,4);
    verticalLayout->addWidget(exitButton,0,5);


    verticalLayout->addWidget(zhanKaiLine, 0, 7);
    verticalLayout->addWidget(zhanKaiBtn, 0, 8);
    verticalLayout->addWidget(qieHuan, 0, 9);
    // 添加布局到主布局
    mainLayout->addLayout(verticalLayout, 0, 0, 1, 10);
    topButtonLayout->addStretch(1); // Add space after buttons


    // 添加工作模式分组
    workModeBox = new QGroupBox("工作方式", this);
    QHBoxLayout *workModeLayout = new QHBoxLayout(workModeBox);  // Changed to QHBoxLayout
    putTongMode = new QRadioButton("普通模式", this);
    weiHuMode = new QRadioButton("维护模式", this);
    saoMiaoTime = new QLabel("扫描时间：", this);
    danWeiMiao = new QLabel("单位0.1秒", this);
    tuBianSetLabel = new QLabel("突变跨度设置：", this);
    workModeLayout->addWidget(putTongMode);
    workModeLayout->addWidget(weiHuMode);
    putTongMode->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    weiHuMode->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(workModeBox, 1, 0);

    mainLayout->addWidget(xiuZhengLabel, 1, 1);
    mainLayout->addWidget(xiuZhengLine, 1, 2);
    mainLayout->addWidget(xiuZhengBtn, 1, 3);
    mainLayout->addWidget(saoMiaoTime, 1, 4);
    mainLayout->addWidget(saoMiaoData, 1, 5);
    mainLayout->addWidget(danWeiMiao, 1, 6);
    mainLayout->addWidget(saveBtn, 1, 7);
    mainLayout->addWidget(tuBianSetLabel, 1, 8);
    mainLayout->addWidget(tuBianSet, 1, 9);
    mainLayout->addWidget(saveTuBianBtn, 1, 10);
    xiuZhengLabel->setStyleSheet("QLabel { margin-top: 32px; margin-bottom: 17px; }");
    xiuZhengLine->setStyleSheet("QLineEdit { margin-top: 32px; margin-bottom: 17px; }");
    xiuZhengBtn->setStyleSheet("QPushButton { margin-top: 32px; margin-bottom: 17px; }");
    saoMiaoTime->setStyleSheet("QLabel { margin-top: 32px; margin-bottom: 17px; }");
    saoMiaoData->setStyleSheet("QSpinBox { margin-top: 32px; margin-bottom: 17px; }");
    danWeiMiao->setStyleSheet("QLabel { margin-top: 32px; margin-bottom: 17px; }");
    saveBtn->setStyleSheet("QPushButton { margin-top: 32px; margin-bottom: 17px; }");
    tuBianSetLabel->setStyleSheet("QLabel { margin-top: 32px; margin-bottom: 17px; }");
    tuBianSet->setStyleSheet("QLineEdit { margin-top: 32px; margin-bottom: 17px; }");
    saveTuBianBtn->setStyleSheet("QPushButton { margin-top: 32px; margin-bottom: 17px; }");


    // 添加创建记录区
    recordCreationBox = new QGroupBox("创建区", this);
    QGridLayout *recordLayout = new QGridLayout(recordCreationBox);


    // 添加控件到布局
    recordLayout->addWidget(new QLabel("悬挂件名称：", this), 0, 0);
    recordLayout->addWidget(xuanGuaName, 0, 1);
    recordLayout->addWidget(new QLabel("压装日期：", this), 0, 2);
    recordLayout->addWidget(yaZhuangData, 0, 3);
    recordLayout->addWidget(new QLabel("操作者：", this), 0, 4);
    recordLayout->addWidget(caoZuoName, 0, 5);
    recordLayout->addWidget(new QLabel("检查者：", this), 0, 6);
    recordLayout->addWidget(jianChaName, 0, 7);
    recordLayout->addWidget(createJiLu, 0, 8);

    // 设置布局间距和外边距
    recordLayout->setSpacing(13);
    recordLayout->setContentsMargins(15, 15, 15, 15);

    // 设置控件的自适应大小策略
    xuanGuaName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    xiuZhengLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    xiuZhengBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    yaZhuangData->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    saoMiaoData->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    saveBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    caoZuoName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tuBianSet->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    saveTuBianBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    jianChaName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    zhanKaiLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    zhanKaiBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    createJiLu->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // 添加到主布局
    mainLayout->addWidget(recordCreationBox, 2, 0, 1, 11);

    // 添加数据区
    shuJuBox= new QCheckBox("数据点", this);
    jieShu1 = new QPushButton("结束", this);
    jieShu2 = new QPushButton("结束", this);
    shuJuBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    jieShu1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    jieShu2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    startReBtn1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    startReBtn2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    chartBox = new QGroupBox("图表区域", this);
    chartLayout = new QGridLayout(chartBox);
    chartView1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    // 设置图表的大小策略，以确保它们在布局中正确显示
    chartLayout->addWidget(shuJuBox, 0, 0);          // Column 0: 数据点 selection box
    chartLayout->addWidget(startReBtn1, 0, 1);       // Column 1: Start Receive Button 1
    chartLayout->addWidget(jieShu1, 0, 2);           // Column 2: Stop Button 1
    chartLayout->addWidget(startReBtn2, 0, 4);       // Column 3: Start Receive Button 2
    chartLayout->addWidget(jieShu2, 0, 5);           // Column 4: Stop Button 2

    // Add widgets to the grid layout (Row 1: Charts)
    chartLayout->addWidget(chartView1, 1, 0, 1, 3);  // Chart 1: Spans 2 columns (Column 0-1)
    chartLayout->addWidget(chartView2, 1, 3, 1, 3);  // Chart 2: Spans 3 columns (Column 2-4)
    /***************bash20241213*************/
#if 1
        MainWindow::configureChart(chartView1, chart,series1, axisX1, axisY1, "压力曲线1");
        MainWindow::configureChart(chartView2, chart02,series2, axisX2, axisY2, "压力曲线2");
#endif
    /***************bash20241213*************/




    // Set stretch factors for better layout control
    chartLayout->setColumnStretch(0, 1);             // Column 0: Normal stretch
    chartLayout->setColumnStretch(1, 1);             // Column 1: Normal stretch
    chartLayout->setColumnStretch(2, 1);             // Column 2: Normal stretch
    chartLayout->setColumnStretch(3, 1);             // Column 3: Normal stretch
    chartLayout->setColumnStretch(4, 1);             // Column 4: Normal stretch
    chartLayout->setColumnStretch(5, 1);             // Column 5: Normal stretch
    chartLayout->setRowStretch(0, 1);                // Row 0: Less height (controls area)
    chartLayout->setRowStretch(1, 18);                // Row 1: More height (charts area)


    chartView1->chart()->addSeries(series1);
    chartView2->chart()->addSeries(series2);

    // 将图表区域添加到主布局中
    mainLayout->addWidget(chartBox, 3, 0,1,11);
    // === 设置拉伸因子 === //
    mainLayout->setRowStretch(0, 2); // 顶部按钮区占高度的 1 份
    //mainLayout->setRowStretch(1, 2); // “创建记录”区和功能区域占高度的 2 份
    mainLayout->setRowStretch(2, 2); // “创建记录”区和功能区域占高度的 2 份
    mainLayout->setRowStretch(3, 16); // 数据和图表区域占高度的 4 份
    mainLayout->setRowStretch(4, 3); // 底部控制区占高度的 2 份

    // 设置主布局的边距和控件间距
    mainLayout->setContentsMargins(5, 5, 5, 5); // 设置上下左右的边距
    mainLayout->setSpacing(10);                 // 控件之间的间距

    // 设置图表区域样式
    chartBox->setStyleSheet("QGroupBox { background-color: #ecf0f1; border: 2px solid #bdc3c7; border-radius: 8px; padding: 10px; }"
                            "QChartView { border: 1px solid #bdc3c7; border-radius: 8px; }");

    // 确保图表区域占据正确的空间
    chartBox->setLayout(chartLayout);

    // 设置图表区域样式
    chartBox->setStyleSheet("QGroupBox { background-color: #ecf0f1; border: 2px solid #bdc3c7; border-radius: 8px; padding: 10px; }"
                            "QChartView { border: 1px solid #bdc3c7; border-radius: 8px; }");

    #if 1
    // 添加控制区
    controlBox = new QGroupBox("控制区", this);
    QGridLayout *controlLayout = new QGridLayout(controlBox);
    controlBox->setStyleSheet("QGroupBox { font-weight: bold; font-size: 16px; border: 2px solid #3498db; border-radius: 5px; padding: 10px; }");

    controlLayout->addWidget(new QLabel("节点序列号：", this), 0, 0);
    controlLayout->addWidget(jieDianSignLine1, 0, 1);
    controlLayout->addWidget(new QLabel("压装力值：", this), 0, 2);
    controlLayout->addWidget(yaZhuang1, 0, 3);
    controlLayout->addWidget(daYinChartBtn1, 0, 4);


    controlLayout->addWidget(new QLabel("压装结果：", this), 1, 0);
    controlLayout->addWidget(yaZhuangSaultLine1, 1, 1);
    controlLayout->addWidget(new QLabel("压装力标准：", this), 1, 2);
    controlLayout->addWidget(yaZhuangStdLine1, 1, 3);


    controlLayout->addWidget(new QLabel("节点序列号：", this), 0, 5);
    controlLayout->addWidget(jieDianSignLine2, 0, 6);
    controlLayout->addWidget(new QLabel("压装力值：", this), 0, 7);
    controlLayout->addWidget(yaZhuang2, 0, 8);
    controlLayout->addWidget(daYinChartBtn2, 0, 9);

    controlLayout->addWidget(new QLabel("压装结果：", this), 1, 5);
    controlLayout->addWidget(yaZhuangSaultLine2, 1, 6);
    controlLayout->addWidget(new QLabel("压装力标准：", this), 1, 7);
    controlLayout->addWidget(yaZhuangStdLine2, 1, 8);

    wanCheng = new QPushButton("完成并保存曲线", this);
    QPushButton *daYinChart2 = new QPushButton("打印图表2", this);
    QPushButton *zhiJieExit = new QPushButton("直接退出", this);
    fengMingQi = new QPushButton("蜂鸣器", this);
    wanCheng->setStyleSheet(buttonStyle);
    daYinChart2->setStyleSheet(buttonStyle);
    zhiJieExit->setStyleSheet(buttonStyle);
    fengMingQi->setStyleSheet(buttonStyle);

    controlLayout->addWidget(wanCheng, 2, 0);
    controlLayout->addWidget(daYinChart2, 2, 2);
    controlLayout->addWidget(zhiJieExit, 2, 4);
    controlLayout->addWidget(fengMingQi, 2, 7);

    // 设置大小策略
    jieDianSignLine1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    yaZhuang1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    daYinChartBtn1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    jieDianSignLine2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    yaZhuang2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    daYinChartBtn2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    yaZhuangSaultLine1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    yaZhuangStdLine1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    yaZhuangSaultLine2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    yaZhuangStdLine2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // 将控制区添加到主布局
    mainLayout->addWidget(controlBox, 4, 0, 1, 11);
    #endif
}

/**
 * @brief 处理每次点击开始接收按钮时清除之前的表
 * @param QChartView 图表
 */
void MainWindow::clearChart(QChartView *chartView) {
    if (chartView) {
        QChart *chart = chartView->chart();

        // 移除所有数据系列
        QList<QAbstractSeries*> seriesList = chart->series();
        for (QAbstractSeries *series : seriesList) {
            chart->removeSeries(series);
        }

        // 移除所有坐标轴
        QList<QAbstractAxis*> axisList = chart->axes();
        for (QAbstractAxis* axis : axisList) {
            chart->removeAxis(axis);
        }
    }
}

/**
 * @brief 处理两个图表间共用一个进程id
 */
void MainWindow::stopTimers() {
    if (Timer1 != -1) {
        killTimer(Timer1);
        Timer1 = -1;  // 重置定时器ID
    }

    if (Timer2 != -1) {
        killTimer(Timer2);
        Timer2 = -1;  // 重置定时器ID
    }
    qDebug() << "Both timers stopped and IDs reset.";
}

/**
 * @brief 加载外部qss
 */
void MainWindow::applyStyles(QWidget *widget,QString stylesheet)
{
    widget->setStyleSheet(stylesheet); // 使用上面读取到的stylesheet
    // 遍历所有子控件并递归应用样式
    for (auto child : widget->findChildren<QWidget*>()) {
        applyStyles(child,stylesheet);
    }
}


/**
 * @brief 导出pdf
 */
#if 0
void MainWindow::exportPdf()
{
    // 获取数据
    QString _xuanGuaName = xuanGuaName->currentText(); // 悬挂名称
    QString _yaZhuangData = yaZhuangData->text();      // 压装时期
    QString _caoZuoName = caoZuoName->currentText();   // 操作者名称
    QString _jianChaName = jianChaName->currentText(); // 检查者名称

    QString _jieDianSignLine1 = jieDianSignLine1->text(); // 节点序列号
    QString _yaZhuang1 = yaZhuang1->text();               // 压装力值
    QString _yaZhuangSaultLine1 = yaZhuangSaultLine1->text(); // 压装结果
    QString _yaZhuangStdLine1 = yaZhuangStdLine1->text(); // 压装力标准

    QString _jieDianSignLine2 = jieDianSignLine2->text(); // 节点序列号
    QString _yaZhuang2 = yaZhuang2->text();               // 压装力值
    QString _yaZhuangSaultLine2 = yaZhuangSaultLine2->text(); // 压装结果
    QString _yaZhuangStdLine2 = yaZhuangStdLine2->text(); // 压装力标准

    // 设置默认保存路径
    QString defaultPath = QCoreApplication::applicationDirPath() + "/Chart12.pdf";  // 默认保存路径为应用程序目录下

    // 如果文件没有后缀，则添加 .pdf 后缀
    if (QFileInfo(defaultPath).suffix().isEmpty()) {
        defaultPath.append(".pdf");
        qDebug() << defaultPath.append(".pdf") << endl;
    }

    // 创建 QPdfWriter 对象并设置 PDF 输出路径
    QPdfWriter pdfWriter(defaultPath);
    pdfWriter.setPageSize(QPagedPaintDevice::A4);
    pdfWriter.setPageSizeMM(QSizeF(297, 210));

    // 打开modbus.ini文件并指定为ini格式
    QSettings configIni("./chartsSize.ini", QSettings::IniFormat);
    // 读取配置项
    int chartsW = configIni.value("Charts/W", 720).toInt();  // 默认为 "192.168.1.1"
    int chartsH = configIni.value("Charts/H", 640).toInt();  // 默认为 502
    int tableW2 = configIni.value("Charts/W3", 165).toInt();  // 默认为 3000
    int tableH2 = configIni.value("Charts/H3", 180).toInt();  // 默认为 3000

    // 创建 QTextDocument 来保存 HTML 内容
    QTextDocument textDocument;

    // 清空 HTML 内容，准备添加新内容
    QString m_html;
    m_html.append("<h1 style='text-align:center;'>转向架悬挂件节点压装力曲线</h1><br />");

    // 添加 T1
    m_html.append("<table border='5' cellspacing='0' cellpadding='3' width='100%'>");
    m_html.append("<tr>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>悬挂名称</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>" + _xuanGuaName + "</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>" + _yaZhuangData + "</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>操作者</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>" + _caoZuoName + "</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>检查者</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>" + _jianChaName + "</td>");
    m_html.append("</tr>");

    m_html.append("</table><br /><br />");

    // 横向排布图片1和图片2 在 T2 和 T3 之前
    m_html.append("<table border='0' cellspacing='0' cellpadding='0' style='width: 100%;'>");
    m_html.append("<tr>");
#if 1
    // 图片1 动态设置宽度和固定高度
      m_html.append("<td style='width: 48%;'><img src='./chart1.png' width='" + QString::number(chartsW) + "' height='" + QString::number(chartsH) + "'></td>");
      // 图片2 动态设置宽度和固定高度
      m_html.append("<td style='width: 4%;'></td>");  // 空隙列，调整宽度控制图片间距
      m_html.append("<td style='width: 48%;'><img src='./chart2.png' width='" + QString::number(chartsW) + "' height='" + QString::number(chartsH) + "'></td>");
#endif
      m_html.append("</tr>");
    m_html.append("</table><br />");

    // 使用父表格来排列 T2 和 T3 横向显示
    m_html.append("<table border='0' cellspacing='0' cellpadding='0' style='width: 100%;'>");
    m_html.append("<tr>");

    // T2 表格
    m_html.append("<td style='width: 48%; padding-left: 15%; padding-right: 50%'>");
    m_html.append("<table border='5' cellspacing='0' cellpadding='3' width='100%'>");
    m_html.append("<tr><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>节点序列号</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _jieDianSignLine1 + "</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装力值</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuang1 + "</td></tr>");
    m_html.append("<tr><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装结果</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuangSaultLine1 + "</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装力标准</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuangStdLine1 + "</td></tr>");
    m_html.append("</table>");
    m_html.append("</td>");

    // 空隙列（增大宽度来加大间距）
    m_html.append("<td style='width: 4%;'></td>");  // 增加间距，控制T2与T3之间的距离

    // T3 表格
    m_html.append("<td style='width: 48%;'>");
    m_html.append("<table border='5' cellspacing='0' cellpadding='3' width='100%'>");
    m_html.append("<tr><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>节点序列号</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _jieDianSignLine2 + "</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装力值</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuang2 + "</td></tr>");
    m_html.append("<tr><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装结果</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuangSaultLine2 + "</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装力标准</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuangStdLine2 + "</td></tr>");
    m_html.append("</table>");
    m_html.append("</td>");

    m_html.append("</tr>");
    m_html.append("</table>");

    // 将 HTML 内容设置到 QTextDocument
    textDocument.setHtml(m_html);

    // 使用 QPdfWriter 打印 PDF
    textDocument.print(&pdfWriter);

    // 关闭文件
    pdfWriter.newPage();  // 如果需要新页面，使用 newPage
}
#endif

/**
 * @brief 保存图表1、2
 */
#if 1 // old
void MainWindow::saveChartToImage(QChartView* chartView, const QString& filePath)
{
    // 获取图表的当前尺寸
    QSize chartSize = chartView->size();  // 使用当前图表的尺寸作为基础

    // 创建一个新的 QPixmap 用于保存图像
    QPixmap pixmap(chartSize);

    // 使用 QPainter 在 QPixmap 上绘制图表
    QPainter painter(&pixmap);
    pixmap.fill(Qt::white);  // 填充背景为白色

    // 设置高质量渲染
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 绘制图表
    chartView->render(&painter);

    // 保存图像
    if (pixmap.toImage().save(filePath)) {
        qDebug() << "图像保存成功：" << filePath;
    } else {
        qDebug() << "图像保存失败：" << filePath;
    }
}
#endif


void MainWindow::processChartsData(const QList<QVariant> &chartsData) {
    // 使用 QMap 来存储时间（包含时分秒）和多个压力值的键值对
    leftData.clear();
    rightData.clear();
    // 遍历 chartsData，按规则拆分数据
    for (int i = 0; i < chartsData.size(); i += 3) {
        // 每次取 3 个数据组成一组
        QString pressureValue = chartsData[i].toString();   // 压力值
        QString dateTime = chartsData[i + 1].toString();     // 日期时间
        QString dataType = chartsData[i + 2].toString();     // 数据类型 (leftData 或 rightData)

        // 将完整的日期时间 (包含时分秒) 作为键
        QString fullDateTime = dateTime.split(" ").at(0) + " " + dateTime.split(" ").at(1);  // 保留日期和时分秒

        // 将压力值和日期时间存入相应的容器
        if (dataType == "leftData") {
            leftData[fullDateTime].append(pressureValue);  // 将压力值添加到相应日期时间下
        } else if (dataType == "rightData") {
            rightData[fullDateTime].append(pressureValue);  // 将压力值添加到相应日期时间下
        }
    }

//    // 打印左表和右表的数据
//    qDebug() << "Left Data: ";
//    for (auto it = leftData.begin(); it != leftData.end(); ++it) {
//        qDebug() << it.key() << " : " << it.value();
//    }

//    qDebug() << "Right Data: ";
//    for (auto it = rightData.begin(); it != rightData.end(); ++it) {
//        qDebug() << it.key() << " : " << it.value();
//    }
}


// 数据点处理函数
void MainWindow::onCheckBoxToggled(bool checked)
{
    if (checked) {
        // 执行勾选时的操作

        // 显示数据点
        series1->setPointsVisible(true);
        series2->setPointsVisible(true);

        // 设置数据点的颜色为红色
        series1->setPointLabelsColor(Qt::red);

        series2->setPointLabelsColor(Qt::red);

    } else {
        // 执行取消勾选时的操作

        // 隐藏数据点
        series1->setPointsVisible(false);
        series2->setPointsVisible(false);

        // 设置数据点的颜色为默认颜色
        series1->setPointLabelsColor(Qt::black);
        series2->setPointLabelsColor(Qt::black);
    }

    // 更新图表，确保颜色和数据点的变化生效
    chart->update();
    chart02->update();
}


// 初始化图表


#if 1           /***********************bash20241226***********************/
void MainWindow::createChartView123(QChartView *chartView, const QList<QList<QString>> &data, const QString &curveName,
                                    QLineSeries *series, QChart *chart, QDateTimeAxis *axisX, QValueAxis *axisY)
{
    if (!chart->series().contains(series)) {
        chart->addSeries(series);
    }

    // 设置系列可见，清空数据
    series->setVisible(true);
    series->clear();
    axisX->setLabelsVisible(true);
    axisY->setLabelsVisible(true);

    // 计算数据点的数量
    int dataSize = data.size();
    int startIndex = (dataSize > 4) ? dataSize - 4 : 0;  // 如果数据大于4，只显示最后4个数据点

    // 添加数据点（所有数据点或最后四个数据点）
    for (int i = startIndex; i < dataSize; ++i) {
        const auto &dataPair = data[i];
        if (dataPair.size() == 2) {
            const QString &timeString = dataPair[0];
            const double pressureValue = dataPair[1].toDouble();
            QDateTime dateTime = QDateTime::fromString(timeString, "yyyy-MM-dd HH:mm:ss");

            if (dateTime.isValid()) {
                series->append(dateTime.toMSecsSinceEpoch(), pressureValue);
            } else {
                qDebug() << "Invalid time: " << timeString;
            }
        } else {
            qDebug() << "Invalid data pair: " << dataPair;
        }
    }

    // 设置曲线的笔（线条颜色和宽度）
    QPen pen1(Qt::blue);  // 设置曲线颜色为蓝色
    pen1.setWidth(2);     // 设置线条宽度为3px
    series->setPen(pen1); // 应用到数据系列

    // 设置X轴格式
    // 20241229
    axisX->setTitleText("时间");
    // 20241229
    axisX->setFormat("yyyy-MM-dd HH:mm:ss");

    // 设置X轴范围
    if (!series->points().isEmpty()) {
        QDateTime minTime = QDateTime::fromMSecsSinceEpoch(series->points().first().x());
        QDateTime maxTime = QDateTime::fromMSecsSinceEpoch(series->points().last().x());
        axisX->setRange(minTime, maxTime);

        // 设置X轴的刻度数量：直接根据数据点的数量来设置
        axisX->setTickCount(dataSize <= 4 ? dataSize : 4); // 如果数据点小于或等于4，设置为数据点的数量；如果大于4，设置为4个刻度
    }

    // 设置Y轴范围
    axisY->setRange(0, 450);
    // 20241229
    axisY->setTitleText("压力值");
    // 20241229
    axisY->setTickCount(10);
    axisY->setLabelFormat("%.0f");

    // 设置X、Y轴线条样式
    QPen axisPen;
    axisPen.setWidth(2);
    axisPen.setColor(QColor(211, 211, 211));  // 浅灰色
    axisX->setLinePen(axisPen);
    axisY->setLinePen(axisPen);

    // 设置字体样式
    QFont fontX("Arial", 6);
    QFont fontY("Arial", 6);
    fontX.setBold(true);
    fontY.setBold(true);
    axisX->setLabelsFont(fontX);
    axisY->setLabelsFont(fontY);

    // 确保轴添加一次
    if (!chart->axes(Qt::Horizontal).contains(axisX)) {
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
    }
    if (!chart->axes(Qt::Vertical).contains(axisY)) {
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
    }
/*----------------------20241228----------------------------*/
    // 设置标题字体和颜色
    QFont titleFont;
    titleFont.setPointSize(8); // 固定标题字体大小为 8
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(Qt::blue)); // 字体颜色设置为蓝色
    chart->setTitle(curveName); // 设置标题
/*----------------------20241228----------------------------*/
    // 更新图表并显示
    chart->update();
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

#endif          /***********************bash20241226***********************/

#if 1 // 优化
void MainWindow::configureChart(QChartView* chartView, QChart* chart, QLineSeries* series,
                                QDateTimeAxis* axisX, QValueAxis* axisY, const QString& chartTitle) {
    // 确保传入的参数有效
    if (!chartView || !chart || !series || !axisX || !axisY) {
        qWarning() << "Invalid arguments passed to configureChart.";
        return;
    }

    // 设置 Chart 的背景
    chart->setBackgroundPen(Qt::NoPen);  // 移除边框

    // 设置坐标轴范围和刻度
    QDateTime now = QDateTime::currentDateTime();
    axisX->setRange(now.addSecs(-1), now.addSecs(1));  // 设置 X 轴范围：-1 秒到 1 秒
    axisX->setFormat("ss");  // 显示秒数差值
    axisY->setRange(-0.1, 0.1);  // 设置 Y 轴范围：-1 到 1

    // 隐藏坐标轴刻度和标签
    axisX->setLabelsVisible(false);
    axisY->setLabelsVisible(false);
    axisX->setTickCount(3);  // 仅显示中心点
    axisY->setTickCount(3);

    // 设置标题字体和颜色
    QFont titleFont;
    titleFont.setPointSize(8); // 固定标题字体大小为 8
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(Qt::blue)); // 字体颜色设置为蓝色
    chart->setTitle(chartTitle); // 设置标题

    // 设置网格线样式
    QPen gridLinePen(QColor(200, 200, 200));  // 设置网格线颜色为灰色
    gridLinePen.setWidth(2);  // 设置网格线宽度为 2
    gridLinePen.setStyle(Qt::DashLine);  // 设置网格线样式为虚线
    axisX->setGridLinePen(gridLinePen);  // 设置 X 轴网格线样式
    axisY->setGridLinePen(gridLinePen);  // 设置 Y 轴网格线样式
    axisX->setGridLineVisible(true);  // 启用 X 轴网格线
    axisY->setGridLineVisible(true);  // 启用 Y 轴网格线

    // 设置坐标轴标题
    // 20241229
    axisX->setTitleText("0");
    axisY->setTitleText("0");
    // 20241229

    // 隐藏数据系列并设置样式
    // 20241229
    series->setVisible(false);  // 隐藏数据系列
//     series->setVisible(true);  // 隐藏数据系列
    // 20241229

    // 如果 X 轴还没有添加到图表，则添加
    if (!chart->axes(Qt::Horizontal).contains(axisX)) {
        chart->addAxis(axisX, Qt::AlignBottom);  // 添加 X 轴到底部
    }

    // 如果 Y 轴还没有添加到图表，则添加
    if (!chart->axes(Qt::Vertical).contains(axisY)) {
        chart->addAxis(axisY, Qt::AlignLeft);  // 添加 Y 轴到左侧
    }

    // 确保系列只被添加一次
    if (!chart->series().contains(series)) {
        chart->addSeries(series);
    }

    // 确保系列和坐标轴的绑定只发生一次
    if (!series->attachedAxes().contains(axisX)) {
        series->attachAxis(axisX);  // 只在未附加的情况下绑定 X 轴
    }

    if (!series->attachedAxes().contains(axisY)) {
        series->attachAxis(axisY);  // 只在未附加的情况下绑定 Y 轴
    }
#if 1 // 20241229
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->layout()->setContentsMargins(0, 0, 0, 0); // 设置图表布局内边距为 0
    chartView->setContentsMargins(0, 0, 0, 0);
    chartView->setStyleSheet("padding: 0px; margin: 0px;");
#endif

    // 设置 Chart 到 ChartView
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
}

#endif
// 20241229
#if 1
QString MainWindow::generateSimpleSvgChart() {
    QString svg = "<svg width='485' height='370' xmlns='http://www.w3.org/2000/svg' style='background: #f5f5f5;'>";

    // 绘制加粗的 X 和 Y 轴
    svg += "<line x1='50' y1='10' x2='50' y2='350' stroke='black' stroke-width='2'/>"; // Y轴
    svg += "<line x1='50' y1='350' x2='550' y2='350' stroke='black' stroke-width='2'/>"; // X轴

    // Y轴刻度：绘制大刻度及其延伸线，每个大刻度间隔50
    int largeTickSpacing = 50; // 大刻度之间的间距对应的实际数值
    int pixelSpacing = 30;     // 大刻度之间的像素距离
    int smallTickCount = 3;    // 每个大刻度之间的分段数

    for (int i = 0; i <= 10; ++i) { // 绘制10个大刻度
        int yPos = 350 - (i * pixelSpacing); // Y轴刻度的 Y 位置 (注意范围：350~10)

        // 绘制 Y 轴大刻度线
        svg += "<line x1='45' y1='" + QString::number(yPos) + "' x2='50' y2='" + QString::number(yPos) + "' stroke='black' stroke-width='1'/>"; // 大刻度线
        svg += "<text x='20' y='" + QString::number(yPos + 5) + "' font-size='12' font-weight='bold' fill='black'>" + QString::number(i * largeTickSpacing) + "</text>"; // 大刻度值加粗为黑色

        // 添加水平延伸线，实线，黑色
        svg += "<line x1='50' y1='" + QString::number(yPos) + "' x2='550' y2='" + QString::number(yPos) + "' stroke='black' stroke-width='1'/>"; // 实线

        // 绘制小刻度线 (每个大刻度之间分为3个小刻度)
        if (i < 10) { // 小刻度只绘制在大刻度之间
            for (int j = 1; j <= smallTickCount; ++j) {
                int smallYPos = yPos - j * (pixelSpacing / (smallTickCount + 1)); // 小刻度位置 (均分大刻度间距)
                svg += "<line x1='47' y1='" + QString::number(smallYPos) + "' x2='50' y2='" + QString::number(smallYPos) + "' stroke='black' stroke-width='1'/>"; // 小刻度线
            }
        }
    }

    // X轴刻度：模拟时间
    QStringList timeLabels = {"2024-12-13 10:00:00", "2024-12-13 10:01:00", "2024-12-13 10:02:00",
                              "2024-12-13 10:03:00", "2024-12-13 10:04:00"};  // 示例时间刻度
    int xStep = 100; // X轴步长：100像素间隔

    for (int i = 0; i < timeLabels.size(); ++i) {
        int xPos = 50 + i * xStep;

        // 绘制 X 轴刻度线
        svg += "<line x1='" + QString::number(xPos) + "' y1='350' x2='" + QString::number(xPos) + "' y2='355' stroke='black' stroke-width='1'/>"; // 刻度线
        svg += "<text x='" + QString::number(xPos) + "' y='370' font-size='10' font-weight='bold' fill='black' text-anchor='middle'>" + timeLabels[i] + "</text>"; // 时间刻度加粗为黑色

        // 添加垂直延伸线，实线，浅灰色
        svg += "<line x1='" + QString::number(xPos) + "' y1='10' x2='" + QString::number(xPos) + "' y2='350' stroke='black' stroke-width='1'/>"; // 实线
    }

    // 添加数据点和折线（模拟数据）
    QString pathData = "M50 350 L150 230 L250 110 L350 170 L450 50"; // 模拟折线数据
    svg += "<path d='" + pathData + "' stroke='blue' stroke-width='2' fill='none'/>"; // 绘制折线

    svg += "</svg>";

    qDebug() << svg << endl;

    return svg; // 返回纯 SVG 内容
}

void MainWindow::exportPdf()
{
    // 获取数据
    QString _xuanGuaName = xuanGuaName->currentText(); // 悬挂名称
    QString _yaZhuangData = yaZhuangData->text();      // 压装时期
    QString _caoZuoName = caoZuoName->currentText();   // 操作者名称
    QString _jianChaName = jianChaName->currentText(); // 检查者名称

    QString _jieDianSignLine1 = jieDianSignLine1->text(); // 节点序列号
    QString _yaZhuang1 = yaZhuang1->text();               // 压装力值
    QString _yaZhuangSaultLine1 = yaZhuangSaultLine1->text(); // 压装结果
    QString _yaZhuangStdLine1 = yaZhuangStdLine1->text(); // 压装力标准

    QString _jieDianSignLine2 = jieDianSignLine2->text(); // 节点序列号
    QString _yaZhuang2 = yaZhuang2->text();               // 压装力值
    QString _yaZhuangSaultLine2 = yaZhuangSaultLine2->text(); // 压装结果
    QString _yaZhuangStdLine2 = yaZhuangStdLine2->text(); // 压装力标准

    // 设置默认保存路径
    QString defaultPath = QCoreApplication::applicationDirPath() + "/Chart12.pdf";  // 默认保存路径为应用程序目录下

    // 如果文件没有后缀，则添加 .pdf 后缀
    if (QFileInfo(defaultPath).suffix().isEmpty()) {
        defaultPath.append(".pdf");
        qDebug() << defaultPath.append(".pdf") << endl;
    }

    // 创建 QPdfWriter 对象并设置 PDF 输出路径
    QPdfWriter pdfWriter(defaultPath);
    pdfWriter.setPageSize(QPagedPaintDevice::A4);
    pdfWriter.setPageSizeMM(QSizeF(297, 210));

    // 打开modbus.ini文件并指定为ini格式
    QSettings configIni("./chartsSize.ini", QSettings::IniFormat);
    // 读取配置项
    int chartsW = configIni.value("Charts/W", 720).toInt();  // 默认为 "192.168.1.1"
    int chartsH = configIni.value("Charts/H", 640).toInt();  // 默认为 502
    int tableW2 = configIni.value("Charts/W3", 165).toInt();  // 默认为 3000
    int tableH2 = configIni.value("Charts/H3", 180).toInt();  // 默认为 3000

    // 创建 QTextDocument 来保存 HTML 内容
    QTextDocument textDocument;

    // 清空 HTML 内容，准备添加新内容
    QString m_html;
    m_html.append("<h1 style='text-align:center;'>转向架悬挂件节点压装力曲线</h1><br />");

    // 添加 T1
    m_html.append("<table border='5' cellspacing='0' cellpadding='3' width='100%'>");
    m_html.append("<tr>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>悬挂名称</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>" + _xuanGuaName + "</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>" + _yaZhuangData + "</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>操作者</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>" + _caoZuoName + "</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>检查者</td>");
    m_html.append("<td width='14%' style='font-size: " + QString::number(tableH2) + "px' valign='center'>" + _jianChaName + "</td>");
    m_html.append("</tr>");

    m_html.append("</table><br /><br />");

    // 横向排布图片1和图片2 在 T2 和 T3 之前
    m_html.append("<table border='0' cellspacing='0' cellpadding='0' style='width: 100%;'>");
    m_html.append("<tr>");
#if 1
     MainWindow::saveSvgChartAsImage();
     MainWindow::saveSvgChartAsImage2();
    // 图片1 动态设置宽度和固定高度
      m_html.append("<td style='width: 48%;'><img src='./chart11.svg' width='" + QString::number(chartsW) + "' height='" + QString::number(chartsH) + "'></td>");
      // 图片2 动态设置宽度和固定高度
      m_html.append("<td style='width: 4%;'></td>");  // 空隙列，调整宽度控制图片间距
      m_html.append("<td style='width: 48%;'><img src='./chart22.svg' width='" + QString::number(chartsW) + "' height='" + QString::number(chartsH) + "'></td>");
#endif
      m_html.append("</tr>");
    m_html.append("</table><br />");

    // 使用父表格来排列 T2 和 T3 横向显示
    m_html.append("<table border='0' cellspacing='0' cellpadding='0' style='width: 100%;'>");
    m_html.append("<tr>");

    // T2 表格
    m_html.append("<td style='width: 48%; padding-left: 15%; padding-right: 50%'>");
    m_html.append("<table border='5' cellspacing='0' cellpadding='3' width='100%'>");
    m_html.append("<tr><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>节点序列号</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _jieDianSignLine1 + "</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装力值</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuang1 + "</td></tr>");
    m_html.append("<tr><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装结果</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuangSaultLine1 + "</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装力标准</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuangStdLine1 + "</td></tr>");
    m_html.append("</table>");
    m_html.append("</td>");

    // 空隙列（增大宽度来加大间距）
    m_html.append("<td style='width: 4%;'></td>");  // 增加间距，控制T2与T3之间的距离

    // T3 表格
    m_html.append("<td style='width: 48%;'>");
    m_html.append("<table border='5' cellspacing='0' cellpadding='3' width='100%'>");
    m_html.append("<tr><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>节点序列号</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _jieDianSignLine2 + "</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装力值</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuang2 + "</td></tr>");
    m_html.append("<tr><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装结果</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuangSaultLine2 + "</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>压装力标准</td><td width='" + QString::number(tableW2) + "' style='width: 12.5%; font-size: " + QString::number(tableH2) + "px;' valign='center'>" + _yaZhuangStdLine2 + "</td></tr>");
    m_html.append("</table>");
    m_html.append("</td>");

    m_html.append("</tr>");
    m_html.append("</table>");

    // 将 HTML 内容设置到 QTextDocument
    textDocument.setHtml(m_html);

    // 使用 QPdfWriter 打印 PDF
    textDocument.print(&pdfWriter);

    // 关闭文件
    pdfWriter.newPage();  // 如果需要新页面，使用 newPage
}


// 生成曲线1svg
void MainWindow::saveSvgChartAsImage() {
    // 生成 SVG 数据
    QString svgData = MainWindow::generateSimpleSvgChart();

    // 设置保存路径
    QString savePath = "chart11.svg"; // 保存为 SVG 文件

    // 保存 SVG 文件
    QFile file(savePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8"); // 设置编码为 UTF-8
        out << svgData; // 将 SVG 数据写入文件
        file.close();
        qDebug() << "SVG chart saved successfully as:" << savePath;
    } else {
        qDebug() << "Failed to save SVG chart as file.";
    }
}
        /***************bash20241213*************/

// 生产曲线2svg
/***************bash20241213*************/
    void MainWindow::saveSvgChartAsImage2() {
        // 生成 SVG 数据
        QString svgData = MainWindow::generateSimpleSvgChart();

        // 设置保存路径
        QString savePath = "chart22.svg"; // 保存为 SVG 文件

        // 保存 SVG 文件
        QFile file(savePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8"); // 设置编码为 UTF-8
            out << svgData; // 将 SVG 数据写入文件
            file.close();
            qDebug() << "SVG chart saved successfully as:" << savePath;
        } else {
            qDebug() << "Failed to save SVG chart as file.";
        }
    }
/***************bash20241213*************/
#endif
// 20241229

void MainWindow::initStart1(){
#if 1

//            MainWindow::dataBaseConn = SqliteAction::getDatabaseConnection("./D1.db");

#endif
}

/***********************bash20241218***********************/
//创建目录
void MainWindow::createDirectory(const QString& path) {
    QDir dir;

    // mkpath() 会自动创建多级目录
    if (dir.mkpath(path)) {
        qDebug() << "Directory created successfully:" << path;
    } else {
        qDebug() << "Failed to create directory:" << path;
    }
}
/***********************bash20241218***********************/

/*
首先点击打印
如果本地存在Chart12.pdf, 将Chart12.pdf备份到./data/chart/(默认创建)2024-12-26(目录)
*/
/***********************bash20241226***********************/
//实现cp备份命令
void MainWindow::backupChartFile() {
    // 获取当前日期，格式化为 yyyyMMdd
    QString currentDate = QDate::currentDate().toString("yyyyMMdd");

    // 设置标志位（可以根据需要自定义，例：可以是 "v1", "backup", etc.）
    QString flag = "backup";  // 你可以根据需求改变这个标志位

    // 构建新的文件名

    QString sourceFile = "C:/Users/WINQ31/Desktop/item4/build-qtModBus-Desktop_Qt_5_8_0_MinGW_32bit-Release/release/Chart12.pdf";  // 当前目录下的原文件
    QString targetDir = QDir::currentPath() + "/data/chart/"+currentDate;  // 目标目录（当前工作目录下的 data 文件夹）
    qDebug() << "path:" << targetDir << endl;
    QString targetFile = targetDir + currentDate + "_" + flag + ".pdf";  // 新的备份文件名
    qDebug() << "newName:" << targetFile << endl;
    // 确保目标目录存在
    QDir dir(targetDir);
    if (!dir.exists()) {
        dir.mkpath(targetDir);  // 如果 data 目录不存在，则创建它
    }

    // 复制文件
    if (QFile::exists(sourceFile)) {
        if (QFile::copy(sourceFile, targetFile)) {
            qDebug() << "File backed up successfully to:" << targetFile;
        } else {
            qDebug() << "Failed to back up the file.";
        }
    } else {
        qDebug() << "Source file does not exist.";
    }
}

/***********************bash20241226***********************/



