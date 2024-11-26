#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sqliteaction.h"

QPushButton *createJiLu;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置主窗口
            setWindowTitle("悬挂件压装力测试系统");
            resize(1200, 800);
#if 1

    MainWindow::initializeControls();
MainWindow::a = 0;
// 创建控件
QComboBox *xuanGuaName = new QComboBox(this);     // 悬挂件名称
QLineEdit *xiuZhengLine = new QLineEdit(this);    // 修正系数输入框
QPushButton *xiuZhengBtn = new QPushButton("修正曲线", this);  // 修正按钮
QLabel *xiuZhengLabel = new QLabel("修正系数：", this);

QDateEdit *yaZhuangData = new QDateEdit(this);    // 压装日期选择器
yaZhuangData->setDate(QDate::currentDate());

QSpinBox *saoMiaoData = new QSpinBox(this);       // 扫描时间
QPushButton *saveBtn = new QPushButton("保存", this);  // 保存按钮

QComboBox *caoZuoName = new QComboBox(this);      // 操作者下拉框
QLineEdit *tuBianSet = new QLineEdit(this);       // 突变跨度设置
QPushButton *saveTuBianBtn = new QPushButton("保存", this);  // 保存突变按钮

QComboBox *jianChaName = new QComboBox(this);     // 检查者下拉框
QLineEdit *zhanKaiLine = new QLineEdit("0", this);  // 展开值
QPushButton *zhanKaiBtn = new QPushButton("展开", this);  // 展开按钮
// 左侧控件组
QLineEdit *jieDianSignLine1 = new QLineEdit(this);
QPushButton *daYinChartBtn1 = new QPushButton("打印图表", this);
QLineEdit *yaZhuang1 = new QLineEdit(this);
// 左侧第二行
QLineEdit *yaZhuangSaultLine1 = new QLineEdit(this);
QLineEdit *yaZhuangStdLine1 = new QLineEdit(this);
// 右侧控件组（与左侧对称）
QLineEdit *jieDianSignLine2 = new QLineEdit(this);
QPushButton *daYinChartBtn2 = new QPushButton("打印图表", this);
QLineEdit *yaZhuang2 = new QLineEdit(this);
// 右侧第二行
QLineEdit *yaZhuangSaultLine2 = new QLineEdit(this);
QLineEdit *yaZhuangStdLine2 = new QLineEdit(this);

// 创建按钮并设置大小策略
QPushButton *loginButton = new QPushButton("登录", this);
QPushButton *logoutButton = new QPushButton("注销", this);
QPushButton *standardButton = new QPushButton("悬挂件/压装力标准值和范围", this);
recordQueryButton = new QPushButton("记录查询", this);
QPushButton *dataMaintenanceButton = new QPushButton("数据维护", this);
QPushButton *exitButton = new QPushButton("退出", this);


    connect(startReBtn1, QPushButton::clicked, this, startRefun1);
    timer->start(1000);
    elapsedTimer->start();

    createJiLu = new QPushButton("创建", this);  // 创建记录按钮
    // 点击创建连接数据库，将主记录信息插入数据库
    connect(createJiLu, QPushButton::clicked,this, [=]{
            qDebug() << "创建记录" << endl;
        MainWindow::dataBaseConn = SqliteAction::getDatabaseConnection("../qtModBus/D1.db");
        bool result1 = SqliteAction::ensureTableInDatabase(dataBaseConn, "../qtModBus/D1.db", "mainListTb");
        if(result1)
            qDebug() << "Table创建成功!!!"<< endl;
        QList<QVariant> values;

        // 将各个控件的数据存入 QList<QVariant>
        values.append(xuanGuaName->currentText());       // 选挂名称
        values.append(yaZhuangData->text());             // 压装日期
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
//qDebug() << xuanGuaName->currentText() << yaZhuangData->text() << caoZuoName->currentText() <<
//         jianChaName->currentText()<<jieDianSignLine1->text() << yaZhuang1->text() <<yaZhuangSaultLine1->text()
//         <<yaZhuangStdLine1->text()<<yaZhuangStdLine1->text() <<jieDianSignLine2->text() <<yaZhuang2->text() <<
//        yaZhuangSaultLine2->text() << yaZhuangStdLine2->text()<< endl;
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
    });
    // 假设这是按钮的连接代码（点击按钮后触发）
    connect(recordQueryButton, &QPushButton::clicked, this, [=] {
        qDebug() << "即将跳转历史页面......";
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

        // 在新窗体中显示查询结果
        MainWindow::w1.show();
    });

#endif


#if 1
// 创建菜单栏
QMenuBar *menuBar = new QMenuBar(this);
setMenuBar(menuBar);
menuBar->addMenu("系统(&V)");
menuBar->addMenu("设置(&Y)");
menuBar->addMenu("维护(&P)");
menuBar->addMenu("工具(&T)");
menuBar->addMenu("帮助(&Z)");

// 设置菜单栏样式
menuBar->setStyleSheet("QMenuBar { background-color: #2c3e50; color: white; font-size: 14px; }"
                       "QMenuBar::item { background-color: #34495e; padding: 5px 10px; }"
                       "QMenuBar::item:selected { background-color: #1abc9c; }");

// 创建主窗口的中心部件
QWidget *centralWidget = new QWidget(this);
setCentralWidget(centralWidget);

// 主布局
QGridLayout *mainLayout = new QGridLayout(centralWidget);
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

// 设置按钮样式
loginButton->setStyleSheet("background-color: #3498db; color: white; border-radius: 5px;");
logoutButton->setStyleSheet("background-color: #e74c3c; color: white; border-radius: 5px;");
standardButton->setStyleSheet("background-color: #2ecc71; color: white; border-radius: 5px;");
recordQueryButton->setStyleSheet("background-color: #f39c12; color: white; border-radius: 5px;");
dataMaintenanceButton->setStyleSheet("background-color: #9b59b6; color: white; border-radius: 5px;");
exitButton->setStyleSheet("background-color: #34495e; color: white; border-radius: 5px;");

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
QLabel *qieHuan = new QLabel("主设备:0\n副设备:0\nF5切换:主设备\n数据来源:仪表",this);

verticalLayout->addWidget(zhanKaiLine, 0, 7);
verticalLayout->addWidget(zhanKaiBtn, 0, 8);
verticalLayout->addWidget(qieHuan, 0, 9);
// 添加布局到主布局
mainLayout->addLayout(verticalLayout, 0, 0, 1, 10);
topButtonLayout->addStretch(1); // Add space after buttons

// 添加工作模式分组
QGroupBox *workModeBox = new QGroupBox("工作方式", this);
QHBoxLayout *workModeLayout = new QHBoxLayout(workModeBox);  // Changed to QHBoxLayout
QRadioButton *putTongMode = new QRadioButton("普通模式", this);
QRadioButton *weiHuMode = new QRadioButton("维护模式", this);
workModeLayout->addWidget(putTongMode);
workModeLayout->addWidget(weiHuMode);
putTongMode->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
weiHuMode->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
mainLayout->addWidget(workModeBox, 1, 0);

mainLayout->addWidget(xiuZhengLabel, 1, 1);
mainLayout->addWidget(xiuZhengLine, 1, 2);
mainLayout->addWidget(xiuZhengBtn, 1, 3);
mainLayout->addWidget(new QLabel("扫描时间：", this), 1, 4);
mainLayout->addWidget(saoMiaoData, 1, 5);
mainLayout->addWidget(new QLabel("单位0.1秒", this), 1, 6);
mainLayout->addWidget(saveBtn, 1, 7);
mainLayout->addWidget(new QLabel("突变跨度设置：", this), 1, 8);
mainLayout->addWidget(tuBianSet, 1, 9);
mainLayout->addWidget(saveTuBianBtn, 1, 10);


// 设置工作模式分组样式
workModeBox->setStyleSheet("QGroupBox { background-color: #ecf0f1; border: 2px solid #bdc3c7; border-radius: 8px; padding: 10px; font-size: 14px; }"
                           "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 10px; background-color: #95a5a6; color: white; font-weight: bold; }"
                           "QRadioButton { font-size: 12px; }"
                           "QRadioButton::indicator { width: 15px; height: 15px; }");
#if 1
// 添加创建记录区
QGroupBox *recordCreationBox = new QGroupBox("创建区", this);
QGridLayout *recordLayout = new QGridLayout(recordCreationBox);




// 填充数据到下拉框
std::vector<QString> xuanGuaItems = {"选项1", "选项2", "选项3", "选项4"};
xuanGuaName->addItems(QStringList::fromVector(QVector<QString>::fromStdVector(xuanGuaItems)));

std::vector<QString> caoZuoItems = {"小马", "小明", "小红", "赵武"};
caoZuoName->addItems(QStringList::fromVector(QVector<QString>::fromStdVector(caoZuoItems)));

std::vector<QString> jianChaItems = {"小马", "小明", "小红", "赵武"};
jianChaName->addItems(QStringList::fromVector(QVector<QString>::fromStdVector(jianChaItems)));

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


//recordLayout->addWidget(xiuZhengLabel, 1, 0);
//recordLayout->addWidget(xiuZhengLine, 1, 1);
//recordLayout->addWidget(xiuZhengBtn, 1, 2);

//recordLayout->addWidget(new QLabel("扫描时间：", this), 1, 3);
//recordLayout->addWidget(saoMiaoData, 1, 4);
//recordLayout->addWidget(saveBtn, 1, 5);
//recordLayout->addWidget(new QLabel("突变跨度设置：", this), 1, 6);
//recordLayout->addWidget(tuBianSet, 1, 7);
//recordLayout->addWidget(saveTuBianBtn, 1, 8);

//recordLayout->addWidget(zhanKaiLine, 1, 9);
//recordLayout->addWidget(zhanKaiBtn, 1, 10);

// 设置布局间距和外边距
recordLayout->setSpacing(13);
recordLayout->setContentsMargins(15, 15, 15, 15);

// 设置样式
recordCreationBox->setFont(QFont("Microsoft YaHei", 10));
recordCreationBox->setStyleSheet(
    "QGroupBox { background-color: #ecf0f1; border: 2px solid #bdc3c7; border-radius: 8px; padding: 10px; }"
    "QLineEdit, QDateEdit, QSpinBox, QComboBox { background-color: #ffffff; border: 1px solid #bdc3c7; border-radius: 5px; padding: 5px; font-size: 12px; }"
    "QPushButton { background-color: #3498db; color: white; border: none; border-radius: 5px; padding: 8px 12px; font-size: 12px; }"
    "QPushButton:hover { background-color: #2980b9; }"
    "QLabel { font-size: 12px; color: #2c3e50; }"
    "QLineEdit:focus { border: 1px solid #2980b9; }"
);

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
//mainLayout->addWidget(recordCreationBox, 1, 1);
#endif



//------------------------------------------------------
// 添加数据区
//QGroupBox *dataBox = new QGroupBox("数据区域", this);
QCheckBox *shuJuBox= new QCheckBox("数据点", this);
QPushButton *jieShu1 = new QPushButton("结束", this);
QPushButton *jieShu2 = new QPushButton("结束", this);
//QPushButton *daYinAll = new QPushButton("打印", this);
//QVBoxLayout *dataLayout = new QVBoxLayout(dataBox);
//dataLayout->addWidget(shuJuBox);
//dataLayout->addWidget(startReBtn1);
//dataLayout->addWidget(jieShu1);
//dataLayout->addWidget(startReBtn2);
//dataLayout->addWidget(jieShu2);
//dataLayout->addWidget(daYinAll);
shuJuBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//startReBtn1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//jieShu1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//startReBtn2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//jieShu2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//daYinAll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//dataLayout->setSpacing(13);
//dataLayout->setContentsMargins(15, 15, 15, 15);
//mainLayout->addWidget(dataBox, 3, 0);

// 设置数据区样式
//dataBox->setStyleSheet("QGroupBox { background-color: #ecf0f1; border: 2px solid #bdc3c7; border-radius: 8px; padding: 10px; }"
//                       "QCheckBox { font-size: 12px; }"
//                       "QPushButton { background-color: #3498db; color: white; border-radius: 5px; padding: 8px 15px; font-size: 12px; }");

#if 1
// 图表区域
QGroupBox *chartBox = new QGroupBox("图表区域", this);
QGridLayout *chartLayout = new QGridLayout(chartBox);

// 创建两个图表并初始化
QChartView *chartView1 = new QChartView();
QChartView *chartView2 = new QChartView();
QValueAxis *axisX1 = new QValueAxis();
QValueAxis *axisY1 = new QValueAxis();
QValueAxis *axisX2 = new QValueAxis();
QValueAxis *axisY2 = new QValueAxis();
chartView1 = createChartView("压力曲线1", axisX1, axisY1);
chartView2 = createChartView("压力曲线2", axisX2, axisY2);

// 设置图表的大小策略，以确保它们在布局中正确显示
//chartView1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//chartView2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
// Add widgets to the grid layout (Row 0: Controls)
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
chartLayout->setRowStretch(1, 5);                // Row 1: More height (charts area)

// Optional: Add spacing for better aesthetics
//chartLayout->setHorizontalSpacing(15);           // Space between columns
//chartLayout->setVerticalSpacing(10);             // Space between rows

// Optional: Set stretch factors for better layout control


/**操作通过plc获取压装力值绘制到曲线上**/
QSplineSeries *series1 = new QSplineSeries();
QSplineSeries *series2 = new QSplineSeries();

chartView1->chart()->addSeries(series1);
chartView2->chart()->addSeries(series2);
// 设置轴
chartView1->chart()->setAxisX(axisX1, series1);
chartView1->chart()->setAxisY(axisY1, series1);

chartView2->chart()->setAxisX(axisX2, series2);
chartView2->chart()->setAxisY(axisY2, series2);

//// 创建定时器 前二十秒读取到的值存入data1中并且同时在ui上显示
//MainWindow::insertDataToSeries(series1, series2, data1);
//  --------测试--------------
// 初始化 data1（全局或类内成员）
data1.resize(1);  // 确保 data1 至少有一个点
#if 0
// 定时器槽函数
connect(timer, &QTimer::timeout, this, [=] {
    int elapsedTime = elapsedTimer->elapsed();
    int elapsedSeconds = elapsedTime / 1000;

    qDebug() << "Elapsed time (s):" << elapsedSeconds;

    startRefun1();

    // 更新 data1 的第一个点
    if (data1.size() > 0) {
        data1[0].setX(elapsedSeconds);
        data1[0].setY(MainWindow::a);
    } else {
        qDebug() << "data1 is empty, resizing...";
        data1.resize(1);
        data1[0] = QPointF(0, 0);
    }

    MainWindow::insertDataToSeries(series1, series2, data1);
});
#endif

#endif

// 将图表区域添加到主布局中
mainLayout->addWidget(chartBox, 3, 0,1,11);
// === 设置拉伸因子 === //
mainLayout->setRowStretch(0, 2); // 顶部按钮区占高度的 1 份
//mainLayout->setRowStretch(1, 2); // “创建记录”区和功能区域占高度的 2 份
mainLayout->setRowStretch(2, 2); // “创建记录”区和功能区域占高度的 2 份
mainLayout->setRowStretch(3, 16); // 数据和图表区域占高度的 4 份
mainLayout->setRowStretch(4, 3); // 底部控制区占高度的 2 份
//mainLayout->setColumnStretch(0, 1); // 左侧功能区占宽度的 1 份
//mainLayout->setColumnStretch(1, 12); // 右侧区域（记录和图表）占宽度的 3 份

// 设置主布局的边距和控件间距
mainLayout->setContentsMargins(5, 5, 5, 5); // 设置上下左右的边距
mainLayout->setSpacing(10);                 // 控件之间的间距

// 设置图表区域样式
chartBox->setStyleSheet("QGroupBox { background-color: #ecf0f1; border: 2px solid #bdc3c7; border-radius: 8px; padding: 10px; }"
                        "QChartView { border: 1px solid #bdc3c7; border-radius: 8px; }");

// 确保图表区域占据正确的空间
chartBox->setLayout(chartLayout);

//// 将图表添加到布局中
//chartLayout->addWidget(chartView1, 0, 0);
//chartLayout->addWidget(chartView2, 0, 1);

// 设置图表区域样式
chartBox->setStyleSheet("QGroupBox { background-color: #ecf0f1; border: 2px solid #bdc3c7; border-radius: 8px; padding: 10px; }"
                        "QChartView { border: 1px solid #bdc3c7; border-radius: 8px; }");



#if 1
// 添加控制区
QGroupBox *controlBox = new QGroupBox("控制区", this);
QGridLayout *controlLayout = new QGridLayout(controlBox);
controlBox->setStyleSheet("QGroupBox { font-weight: bold; font-size: 16px; border: 2px solid #3498db; border-radius: 5px; padding: 10px; }");

// 控件样式
QString lineEditStyle = "QLineEdit { border: 1px solid #7f8c8d; border-radius: 5px; padding: 5px; font-size: 14px; }";
QString buttonStyle = "QPushButton { background-color: #2ecc71; color: white; border-radius: 5px; padding: 8px; font-size: 14px; } QPushButton:hover { background-color: #27ae60; }";
QString labelStyle = "QLabel { font-size: 14px; font-weight: bold; color: #2c3e50; }";



jieDianSignLine1->setStyleSheet(lineEditStyle);
daYinChartBtn1->setStyleSheet(buttonStyle);
yaZhuang1->setStyleSheet(lineEditStyle);

controlLayout->addWidget(new QLabel("节点序列号：", this), 0, 0);
controlLayout->addWidget(jieDianSignLine1, 0, 1);
controlLayout->addWidget(new QLabel("压装力值：", this), 0, 2);
controlLayout->addWidget(yaZhuang1, 0, 3);
controlLayout->addWidget(daYinChartBtn1, 0, 4);

// 设置 QLabel 样式
for (int i = 0; i < controlLayout->count(); ++i) {
    QLabel *label = qobject_cast<QLabel *>(controlLayout->itemAt(i)->widget());
    if (label) {
        label->setStyleSheet(labelStyle);
    }
}



yaZhuangSaultLine1->setStyleSheet(lineEditStyle);
yaZhuangStdLine1->setStyleSheet(lineEditStyle);

controlLayout->addWidget(new QLabel("压装结果：", this), 1, 0);
controlLayout->addWidget(yaZhuangSaultLine1, 1, 1);
controlLayout->addWidget(new QLabel("压装力标准：", this), 1, 2);
controlLayout->addWidget(yaZhuangStdLine1, 1, 3);

jieDianSignLine2->setStyleSheet(lineEditStyle);
daYinChartBtn2->setStyleSheet(buttonStyle);
yaZhuang2->setStyleSheet(lineEditStyle);

controlLayout->addWidget(new QLabel("节点序列号：", this), 0, 5);
controlLayout->addWidget(jieDianSignLine2, 0, 6);
controlLayout->addWidget(new QLabel("压装力值：", this), 0, 7);
controlLayout->addWidget(yaZhuang2, 0, 8);
controlLayout->addWidget(daYinChartBtn2, 0, 9);


yaZhuangSaultLine2->setStyleSheet(lineEditStyle);
yaZhuangStdLine2->setStyleSheet(lineEditStyle);

controlLayout->addWidget(new QLabel("压装结果：", this), 1, 5);
controlLayout->addWidget(yaZhuangSaultLine2, 1, 6);
controlLayout->addWidget(new QLabel("压装力标准：", this), 1, 7);
controlLayout->addWidget(yaZhuangStdLine2, 1, 8);

QPushButton *wanCheng = new QPushButton("完成并保存曲线", this);
QPushButton *daYinChart2 = new QPushButton("打印图表2", this);
QPushButton *zhiJieExit = new QPushButton("直接退出", this);
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

//wanCheng->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//daYinChart2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//zhiJieExit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

// 将控制区添加到主布局
mainLayout->addWidget(controlBox, 4, 0, 1, 11);
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

QChartView* MainWindow::createChartView(const QString &title, QValueAxis *axisX, QValueAxis *axisY) {
     QChart *chart = new QChart();
     chart->setTitle(title);

     QSplineSeries *series = new QSplineSeries();
     series->append(0, 0);

     chart->addSeries(series);
     chart->createDefaultAxes();
     if(title == "压力曲线2")
        axisX->setRange(20, 40);
     else
     axisX->setRange(0, 20);
     axisY->setRange(0, 450);

     chart->setAxisX(axisX, series);
     chart->setAxisY(axisY, series);

     QChartView *chartView = new QChartView(chart);
     chartView->setRenderHint(QPainter::Antialiasing);
     return chartView;
 }

void MainWindow::insertDataToSeries(QSplineSeries *series1, QSplineSeries *series2, const QVector<QPointF> &dataPoints) {
    if (!series1 && !series2) {
//        qDebug() << "Series is null, cannot insert data!";
        return;
    }
    for (const QPointF &point : dataPoints) {
        if (point.x() <= 20) // 直接访问 point 的 x 坐标
            series1->append(point);
        else
            series2->append(point);
    }

}


void MainWindow::recordQueryButtonFun(){
    /**
     * 1、处理点击记录查询按钮弹出记录查询页面
     * 2、按照表格形式 12列显示
     * 3、表格标题主记录列表
     * 4、最顶端有详情明细表 带个下划线
     * 5、最末尾一行是统计消息条数量
    **/

}

void MainWindow::initializeControls()
{
    timer = new QTimer();
    elapsedTimer = new QElapsedTimer(); //记录时间
    startReBtn1 = new QPushButton("开始接收", this);
    startReBtn2 = new QPushButton("开始接收", this);
    yazhuang1 = new QLineEdit(this);
    modbusDevice = new QModbusTcpClient(this);
    xuanGuaName = new QComboBox(this);
    jianChaName = new QComboBox(this);
    caoZuoName = new QComboBox(this);
    yaZhuangData = new QDateEdit(this);

}
