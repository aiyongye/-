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
            MainWindow::dataBaseConn = SqliteAction::getDatabaseConnection("../qtModBus/D1.db");

#if 1

    MainWindow::initializeControls();
MainWindow::a = 0;

QString radioButtonStyle = R"(
    QRadioButton {
        font: 12px;
        color: black;
        spacing: 10px;
    }
)";

// 设置输入框样式
QString lineEditStyle = R"(
    QLineEdit {
        font: bold 20px;
        color: black;
        background-color: white;
        border: 1px solid #A0A0A0;
        border-radius: 5px;
        text-align: center;
    }
)";
QString labelStyle = R"(
    QLabel {
        font: 12px;
        color: black;
        padding: 0 5px; /* 增加水平内间距，避免文字贴边 */
    }
)";

QString comboBoxStyle = R"(
    QComboBox {
        font: 12px;
        color: black;
        background-color: white; /* 白色背景 */
        border: 1px solid #A0A0A0; /* 浅灰色边框 */
        border-radius: 4px; /* 圆角 */
    }
)";
// 设置按钮、输入框、分组框、标签和单选按钮的样式
QString groupBoxStyle = R"(
    QGroupBox {
        background-color: #ecf0f1; /* 浅灰色背景 */
        border: 2px solid #bdc3c7; /* 边框颜色 */
        border-radius: 8px; /* 圆角 */
        padding: 10px; /* 内部间距 */
        margin-top: 15px; /* 标题与边框顶部的距离 */
    }
    QGroupBox::title {
        subcontrol-origin: margin; /* 标题相对于边框外边距 */
        subcontrol-position: top left; /* 标题位置：顶部左侧 */
        padding: 0 10px; /* 标题文字与边框的水平距离 */
        font: bold 14px; /* 加粗字体 */
        color: #2c3e50; /* 深灰色文字 */
        background-color: transparent; /* 标题背景透明 */
    }
    QChartView {
        border: 1px solid #bdc3c7; /* 边框颜色 */
        border-radius: 8px; /* 圆角 */
        background-color: #ffffff; /* 白色背景 */
        padding: 5px; /* 内部留白 */
    }
)";
// 设置按钮的样式，包含按下时颜色变化
buttonStyle = R"(
    QPushButton {
        font: bold 14px;
        color: black;
        background-color: #E5E5E5;
        border: 1px solid #A0A0A0;
        border-radius: 5px;
        padding: 5px;
    }
    QPushButton:pressed {
        background-color: #C8C8C8; /* 按下时变为更深的灰色 */
        border: 1px solid #707070; /* 按下时改变边框颜色 */
    }
    QPushButton:hover {
        background-color: #F2F2F2; /* 鼠标悬停时变为浅灰色 */
    }
)";
 axisX1 = new QDateTimeAxis();
 axisY1 = new QValueAxis();
 axisX2 = new QDateTimeAxis();
 axisY2 = new QValueAxis();
 chart02 = new QChart();
 chart = new QChart();
//timerChart01 = nullptr; // 确保定时器只创建一次
//timerChart02 = nullptr; // 确保定时器只创建一次
 // 创建两个图表并初始化
  chartView1 = new QChartView();
  chartView2 = new QChartView();
// 创建控件
QComboBox *xuanGuaName = new QComboBox(this);     // 悬挂件名称
QLineEdit *xiuZhengLine = new QLineEdit(this);    // 修正系数输入框
QPushButton *xiuZhengBtn = new QPushButton("修正曲线", this);  // 修正按钮
QLabel *xiuZhengLabel = new QLabel("修正系数：", this);

QDateEdit *yaZhuangData = new QDateEdit(this);  // 创建 QDateEdit 控件

// 设置日期显示格式为 "yyyy-MM-dd"
yaZhuangData->setDisplayFormat("yyyy-MM-dd");
// 设置日期为当前日期
yaZhuangData->setDate(QDate::currentDate());


QSpinBox *saoMiaoData = new QSpinBox(this);       // 扫描时间
QPushButton *saveBtn = new QPushButton("保存", this);  // 保存按钮

QComboBox *caoZuoName = new QComboBox(this);      // 操作者下拉框
QLineEdit *tuBianSet = new QLineEdit(this);       // 突变跨度设置
tuBianSet->setText("80");
QPushButton *saveTuBianBtn = new QPushButton("保存", this);  // 保存突变按钮

jianChaName = new QComboBox(this);     // 检查者下拉框
xuanGuaName = new QComboBox(this);

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
QLineEdit *zhanKaiLine = new QLineEdit("0", this);  // 展开值
QPushButton *zhanKaiBtn = new QPushButton("展开", this);  // 展开按钮
QLabel *qieHuan = new QLabel("主设备:0\n副设备:0\nF5切换:主设备\n数据来源:仪表",this);
//QChart *chart = new QChart();
/**操作通过plc获取压装力值绘制到曲线上**/
 series1 = new QLineSeries();
 series2 = new QLineSeries();
    timer->start(1000);
    elapsedTimer->start();
    #if 1
    // 定时器槽函数 用于处理实时显示plc的值
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
    // 处理数据库与控件同步
    // 操作者同步
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
    // 检查者同步
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
        // 假设 dataList2 是包含所有项数据的容器，每个元素是一个 QList<QVariant>，
        // 每个 QList<QVariant> 包含了：xuanName, pressStd1, pressStd2 等信息

        // 连接 QComboBox 的 currentIndexChanged 信号到槽函数
        connect(xuanGuaName, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
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

    createJiLu = new QPushButton("创建", this);  // 创建记录按钮
    // 点击创建连接数据库，将主记录信息插入数据库
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
    connect(standardButton, QPushButton::clicked,this,[=]{
       qDebug() << "即将跳转工艺标准界面" << endl;
       MainWindow::w2.show();
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




qieHuan->setStyleSheet(labelStyle);
zhanKaiLine->setStyleSheet(lineEditStyle);

// 设置展开按钮样式
zhanKaiBtn->setStyleSheet(buttonStyle);

loginButton->setStyleSheet(buttonStyle);
logoutButton->setStyleSheet(buttonStyle);
standardButton->setStyleSheet(buttonStyle);
recordQueryButton->setStyleSheet(buttonStyle);
dataMaintenanceButton->setStyleSheet(buttonStyle);
exitButton->setStyleSheet(buttonStyle);


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
QGroupBox *workModeBox = new QGroupBox("工作方式", this);
QHBoxLayout *workModeLayout = new QHBoxLayout(workModeBox);  // Changed to QHBoxLayout
QRadioButton *putTongMode = new QRadioButton("普通模式", this);
QRadioButton *weiHuMode = new QRadioButton("维护模式", this);
QLabel *saoMiaoTime = new QLabel("扫描时间：", this);
QLabel *danWeiMiao = new QLabel("单位0.1秒", this);
QLabel *tuBianSetLabel = new QLabel("突变跨度设置：", this);
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


// 设置工作模式分组样式
workModeBox->setStyleSheet(groupBoxStyle);
#if 1
// 添加创建记录区
QGroupBox *recordCreationBox = new QGroupBox("创建区", this);
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

// 设置样式
//recordCreationBox->setFont(QFont("Microsoft YaHei", 10));
recordCreationBox->setStyleSheet(groupBoxStyle);


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

// 为控件设置样式
//xuanGuaName->setStyleSheet(comboBoxStyle);
yaZhuangData->setStyleSheet(comboBoxStyle); // 假设是 QComboBox
caoZuoName->setStyleSheet(lineEditStyle);  // 假设是 QLineEdit
jianChaName->setStyleSheet(lineEditStyle); // 假设是 QLineEdit
createJiLu->setStyleSheet(buttonStyle);

// 添加到主布局
mainLayout->addWidget(recordCreationBox, 2, 0, 1, 11);
//mainLayout->addWidget(recordCreationBox, 1, 1);
#endif

// 添加数据区
//QGroupBox *dataBox = new QGroupBox("数据区域", this);
QCheckBox *shuJuBox= new QCheckBox("数据点", this);
QPushButton *jieShu1 = new QPushButton("结束", this);
QPushButton *jieShu2 = new QPushButton("结束", this);
//jieShu1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//jieShu1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

jieShu1->setStyleSheet(buttonStyle);
jieShu2->setStyleSheet(buttonStyle);
shuJuBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
putTongMode->setStyleSheet(radioButtonStyle);
weiHuMode->setStyleSheet(radioButtonStyle);
xiuZhengBtn->setStyleSheet(buttonStyle);
saveBtn->setStyleSheet(buttonStyle);
saveTuBianBtn->setStyleSheet(buttonStyle);
xiuZhengLine->setStyleSheet(lineEditStyle);
saoMiaoData->setStyleSheet(lineEditStyle);
tuBianSet->setStyleSheet(lineEditStyle);
saoMiaoTime->setStyleSheet(labelStyle);
danWeiMiao->setStyleSheet(labelStyle);
tuBianSetLabel->setStyleSheet(labelStyle);
#if 1
// 图表区域
    QGroupBox *chartBox = new QGroupBox("图表区域", this);
    QGridLayout *chartLayout = new QGridLayout(chartBox);
//    chartView1 = createChartView("压力曲线1", axisX1, axisY1);
//    chartView2 = createChartView2("压力曲线2", axisX2, axisY2);

    //////////////////////////////////////////
//  MainWindow::startDataInsertion( axisX1, series1);

connect(startReBtn1, &QPushButton::clicked, this, [=]() {

            MainWindow::stopTimers();
            Timer1 = startTimer(1000);
            MainWindow::clearChart(chartView1);
            chartView1 = createChartView("压力曲线1", axisX1, axisY1);
            chartLayout->addWidget(chartView1, 1, 0, 1, 3);  // Chart 1: Spans 2 columns (Column 0-1)

});
connect(jieShu1, &QPushButton::clicked, this, [=]() {
    // 这里进行判断 和突变值进行比较 如果大于突变值则将大于突变值的值和突变值前三个数
    // 在图表中显示
//    MainWindow::startDataInsertion(axisX1, series1);
    killTimer(Timer1);

    qDebug() << "定时器1 close"<< Timer1 << endl;
});

connect(startReBtn2, &QPushButton::clicked,this,[=]{
            MainWindow::stopTimers();
            Timer2 = startTimer(1000);  // 每秒插入一次数据
            MainWindow::clearChart(chartView2);
            chartView2 = createChartView2("压力曲线2", axisX2, axisY2);
            chartLayout->addWidget(chartView2, 1, 3, 1, 3);  // Chart 2: Spans 3 columns (Column 2-4)


});
connect(jieShu2, &QPushButton::clicked,this,[=]{
            killTimer(Timer2);
            qDebug() << "close" << endl;
});

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
chartLayout->setRowStretch(1, 5);                // Row 1: More height (charts area)


chartView1->chart()->addSeries(series1);
chartView2->chart()->addSeries(series2);


//// 创建定时器 前二十秒读取到的值存入data1中并且同时在ui上显示


#endif

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
QGroupBox *controlBox = new QGroupBox("控制区", this);
QGridLayout *controlLayout = new QGridLayout(controlBox);
controlBox->setStyleSheet("QGroupBox { font-weight: bold; font-size: 16px; border: 2px solid #3498db; border-radius: 5px; padding: 10px; }");

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





void MainWindow::updateChart(QSplineSeries *series) {
    // 清空当前曲线中的数据
    series->clear();
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
    startReBtn1->setStyleSheet(buttonStyle);
    startReBtn2->setStyleSheet(buttonStyle);
    yazhuang1 = new QLineEdit(this);
    modbusDevice = new QModbusTcpClient(this);

    caoZuoName = new QComboBox(this);
    yaZhuangData = new QDateEdit(this);

}

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

// 停止所有定时器
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
