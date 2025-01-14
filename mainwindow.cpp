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
    ui->setupUi(this);
     setAttribute(Qt::WA_QuitOnClose, true);  // 主窗口关闭时退出应用程序
    // 设置主窗口
            setWindowTitle("悬挂件压装力测试系统");
            resize(1200, 800);
            MainWindow::a = 0;

            QFile file(":/mainForm.qss");
            file.open(QFile::ReadOnly);
            QString qss=file.readAll();
            file.close();
            applyStyles(this,qss);

            MainWindow::dataBaseConn = SqliteAction::getDatabaseConnection("../qtModBus/D1.db");
            MainWindow::initializeControls();
            MainWindow::initBuJu();

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
#endif

#if 1
    /**
     * @brief 连接 QComboBox 的 currentIndexChanged 信号到槽函数
     * 单击悬挂名称时会自动将标准显示到标准框里
     */
    connect(xuanGuaName, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        MainWindow::clearChart(chartView1);
        MainWindow::clearChart(chartView2);
        yaZhuang1->setText("");
        yaZhuangSaultLine1->setText("");
        yaZhuang2->setText("");
        yaZhuangSaultLine2->setText("");
        // 如果索引有效
        if (index >= 0 && index < dataList2.size()) {
            // 获取当前选中的项对应的数据行
            const QList<QVariant>& selectedRow = dataList2.at(index);

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
    });
#endif

#if 1
    /**
     * @brief 点击创建连接数据库，将主记录信息插入数据库
     */
    connect(createJiLu, QPushButton::clicked,this, [=]{
            qDebug() << "创建记录" << endl;

        bool result1 = SqliteAction::ensureTableInDatabase(dataBaseConn, "../qtModBus/D1.db", "mainListTb");
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

        bool result2 = SqliteAction::insertIntoTable(dataBaseConn, "../qtModBus/D1.db", "mainListTb", columns, values);

        if(result2)
            qDebug() << "数据插入成功!!!"<< endl;

        // 点击创建获取当前插入主记录中的id
        // streedataList存储了曲线1和2 的压力值曲线时间 标志位
        // 创建表
        bool flags = SqliteAction::streetTableInDatabase(dataBaseConn, "../qtModBus/D1.db", "streetDataTb");
        if(flags)
            qDebug() << "创建streeDataTb成功" << endl;
        // 查询表mainListTb 将最后一条记录的主键值转QString返回
        QString _mainId = SqliteAction::getLastRecordId(dataBaseConn, "mainListTb");
        // 将streedataList容器和主记录id存入数据库中
        flags = SqliteAction::insertStreetData(dataBaseConn, "streetDataTb", streedataList, _mainId);
                if(flags)
                    qDebug() << "数据插入streeDataTb成功" << endl;

    });
#endif

#if 1
    /**
     * @brief 页面跳转到历史界面
     */
    connect(recordQueryButton, &QPushButton::clicked, this, [=] {
        qDebug() << "即将跳转历史页面......";
#if 0
        MainWindow::w1.close();
        // 执行查询：例如查询历史记录
        QList<QList<QVariant>> historyData = SqliteAction::queryTable(dataBaseConn, "../qtModBus/D1.db", "mainListTb");

        // 设定 QTableWidget 的列数和行数
        QTableWidget *tableWidget = MainWindow::w1.findChild<QTableWidget*>("tableWidget2");
        if (!tableWidget) {
            qDebug() << "未找到名为 'tableWidget' 的控件!";
            return;
        }
       int rowCount = historyData.size();
        if (rowCount > 0) {
            int columnCount = historyData[0].size();
            tableWidget->setRowCount(rowCount);
            tableWidget->setColumnCount(columnCount);

            // 填充表格数据
            for (int i = 0; i < rowCount; ++i) {
                const QList<QVariant> &row = historyData[i];
                for (int j = 0; j < columnCount; ++j) {
                    tableWidget->setItem(i, j, new QTableWidgetItem(row[j].toString()));
                }
            }
        }
#endif
        // 在新窗体中显示查询结果
        MainWindow::w1.show();
    });
#endif

#if 1
    /**
     * @brief 页面跳转到设置界面
     */
    connect(standardButton, QPushButton::clicked,this,[=]{
       qDebug() << "即将跳转工艺标准界面" << endl;
       MainWindow::w2.show();
    });
#endif

#if 1


#if 1
    /**
     * @brief 绘制图表1
     */
connect(startReBtn1, &QPushButton::clicked, this, [=]() {

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
            series1->setColor(Qt::red);

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
        // 将 min 和 max 转换为整数
        int minRange = rangeParts[0].toInt();
        int maxRange = rangeParts[1].toInt();

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
            MainWindow::stopTimers();
            Timer2 = startTimer(1000);  // 每秒插入一次数据
            MainWindow::clearChart(chartView2);
            chartView2 = createChartView2("压力曲线2", axisX2, axisY2);
            chartLayout->addWidget(chartView2, 1, 3, 1, 3);  // Chart 2: Spans 3 columns (Column 2-4)
            yaZhuang2->setText("");
            yaZhuangSaultLine2->setText("");
            chart2Container.clear();
            series2->setColor(Qt::red);
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
                int minRange = rangeParts[0].toInt();
                int maxRange = rangeParts[1].toInt();

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
    MainWindow::saveChartToImage(chartView1, "../qtModBus/chart1.png");
    MainWindow::saveChartToImage(chartView2, "../qtModBus/chart2.png");



    MainWindow::exportPdf();
});
#endif

#if 1
    /**
     * @brief 保存图表2成图片
     */
connect(daYinChartBtn1, QPushButton::clicked,this, [=]{

});
#endif

// 状态栏
QStatusBar *statusBar = new QStatusBar(this);
setStatusBar(statusBar);

// 设置状态栏样式
statusBar->setStyleSheet("QStatusBar { background-color: #34495e; color: white; font-size: 12px; }");
#endif
}

MainWindow::~MainWindow()
{
    // 在销毁窗口时关闭数据库连接
       if (dataBaseConn.isOpen()) {
           dataBaseConn.close();
           qDebug() << "Database connection closed!!!.";
       }
    delete ui;
    MainWindow::w1.close();
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
    axisX1 = new QDateTimeAxis();
    axisY1 = new QValueAxis();
    axisX2 = new QDateTimeAxis();
    axisY2 = new QValueAxis();
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
    menuBar->addMenu("系统(&V)");
    menuBar->addMenu("设置(&Y)");
    menuBar->addMenu("维护(&P)");
    menuBar->addMenu("工具(&T)");
    menuBar->addMenu("帮助(&Z)");

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
    xiuZhengLabel->setStyleSheet("QLabel { margin-top: 20px; margin-bottom: 5px; }");
    xiuZhengLine->setStyleSheet("QLineEdit { margin-top: 20px; margin-bottom: 5px; }");
    xiuZhengBtn->setStyleSheet("QPushButton { margin-top: 20px; margin-bottom: 5px; }");
    saoMiaoTime->setStyleSheet("QLabel { margin-top: 20px; margin-bottom: 5px; }");
    saoMiaoData->setStyleSheet("QLineEdit { margin-top: 20px; margin-bottom: 15px; }");
    danWeiMiao->setStyleSheet("QPushButton { margin-top: 20px; margin-bottom: 5px; }");
    saveBtn->setStyleSheet("QPushButton { margin-top: 20px; margin-bottom: 5px; }");
    tuBianSetLabel->setStyleSheet("QLabel { margin-top: 20px; margin-bottom: 5px; }");
    tuBianSet->setStyleSheet("QLineEdit { margin-top: 20px; margin-bottom: 5px; }");
    saveTuBianBtn->setStyleSheet("QPushButton { margin-top: 20px; margin-bottom: 5px; }");


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
    chartLayout->addWidget(startReBtn2, 0, 3);       // Column 3: Start Receive Button 2
    chartLayout->addWidget(jieShu2, 0, 4);           // Column 4: Stop Button 2

    // Add widgets to the grid layout (Row 1: Charts)
    chartLayout->addWidget(chartView1, 1, 0, 1, 3);  // Chart 1: Spans 2 columns (Column 0-1)
    chartLayout->addWidget(chartView2, 1, 3, 1, 3);  // Chart 2: Spans 3 columns (Column 2-4)

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

    QPushButton *wanCheng = new QPushButton("完成并保存曲线", this);
    QPushButton *daYinChart2 = new QPushButton("打印图表2", this);
    QPushButton *zhiJieExit = new QPushButton("直接退出", this);
    wanCheng->setStyleSheet(buttonStyle);
    daYinChart2->setStyleSheet(buttonStyle);
    zhiJieExit->setStyleSheet(buttonStyle);

    controlLayout->addWidget(wanCheng, 2, 0);
    controlLayout->addWidget(daYinChart2, 2, 2);
    controlLayout->addWidget(zhiJieExit, 2, 4);

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
    pdfWriter.setResolution(QPrinter::ScreenResolution);

    // 创建 QTextDocument 来保存 HTML 内容
    QTextDocument textDocument;

    // 清空 HTML 内容，准备添加新内容
    QString m_html;
    m_html.append("<h1 style='text-align:center;'>转向架悬挂件节点压装力曲线</h1><br />");

    // 添加 T1 表格
    m_html.append("<table border='0.5' cellspacing='0' cellpadding='3' width='100%'>");
    m_html.append("<tr><td width='14%' valign='center'>悬挂名称</td><td width='14%' valign='center'>" + _xuanGuaName + "</td><td width='14%' valign='center'>" + _yaZhuangData + "</td><td width='14%' valign='center'>操作者</td><td width='14%' valign='center'>" + _caoZuoName + "</td><td width='14%' valign='center'>检查者</td><td width='14%' valign='center'>" + _jianChaName + "</td></tr>");
    m_html.append("</table><br /><br />");

    // 横向排布图片1和图片2 在 T2 和 T3 之前
    m_html.append("<table border='0' cellspacing='0' cellpadding='0' style='width: 100%;'>");
    m_html.append("<tr>");

    // 图片1
    m_html.append("<td style='width: 48%;'><img src='../qtModBus/chart1.png' width='450' height='400'></td>");

    // 图片2
    m_html.append("<td style='width: 4%;'></td>");  // 空隙列，调整宽度控制图片间距
    m_html.append("<td style='width: 48%;'><img src='../qtModBus/chart2.png' width='450' height='400'></td>");

    m_html.append("</tr>");
    m_html.append("</table><br />");

    // 使用父表格来排列 T2 和 T3 横向显示
    m_html.append("<table border='0' cellspacing='0' cellpadding='0' style='width: 100%;'>");
    m_html.append("<tr>");

    // T2 表格
    m_html.append("<td style='width: 48%; padding-left: 15%; padding-right: 400%'>");
    m_html.append("<table border='0.5' cellspacing='0' cellpadding='3' width='100%'>");
    m_html.append("<tr><td style='width: 12.5%;' valign='center'>节点序列号</td><td style='width: 12.5%;' valign='center'>" + _jieDianSignLine1 + "</td><td style='width: 12.5%;' valign='center'>压装力值</td><td style='width: 12.5%;' valign='center'>" + _yaZhuang1 + "</td></tr>");
    m_html.append("<tr><td style='width: 12.5%;' valign='center'>压装结果</td><td style='width: 12.5%;' valign='center'>" + _yaZhuangSaultLine1 + "</td><td style='width: 12.5%;' valign='center'>压装力标准</td><td style='width: 12.5%;' valign='center'>" + _yaZhuangStdLine1 + "</td></tr>");
    m_html.append("</table>");
    m_html.append("</td>");

    // 空隙列（增大宽度来加大间距）
    m_html.append("<td style='width: 4%;'></td>");  // 增加间距，控制T2与T3之间的距离

    // T3 表格
    m_html.append("<td style='width: 48%;'>");
    m_html.append("<table border='0.5' cellspacing='0' cellpadding='3' width='100%'>");
    m_html.append("<tr><td style='width: 12.5%;' valign='center'>节点序列号</td><td style='width: 12.5%;' valign='center'>" + _jieDianSignLine2 + "</td><td style='width: 12.5%;' valign='center'>压装力值</td><td style='width: 12.5%;' valign='center'>" + _yaZhuang2 + "</td></tr>");
    m_html.append("<tr><td style='width: 12.5%;' valign='center'>压装结果</td><td style='width: 12.5%;' valign='center'>" + _yaZhuangSaultLine2 + "</td><td style='width: 12.5%;' valign='center'>压装力标准</td><td style='width: 12.5%;' valign='center'>" + _yaZhuangStdLine2 + "</td></tr>");
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



/**
 * @brief 保存图表1/2
 */
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
