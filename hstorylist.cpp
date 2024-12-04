#include "hstorylist.h"
#include "ui_hstorylist.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include <QString>

HstoryList::HstoryList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HstoryList)
{
    setAttribute(Qt::WA_QuitOnClose, false);  // 其他窗口关闭时不退出应用程序
    ui->setupUi(this);
    setWindowTitle("记录查询");
    resize(1200, 800);


    dataBaseConn = HstoryList::getDatabaseConnection("./D1.db");

    QFile file(":/historyForm.qss");
    file.open(QFile::ReadOnly);
    QString qss=file.readAll();
    file.close();
    applyStyles(this,qss);

    ui->label_4->setStyleSheet("background-color: #E9EBFE;");
    ui->label_5->setStyleSheet("background-color: #E9EBFE;");

    //// 悬挂件标准同步
    bool flags = HstoryList::queryAllDataFromTableXuan(dataBaseConn, "proStds", dataList2);
        if(flags){
            qDebug() << "查询 inspectorTb 成功!!!";

            // 清空 QComboBox，准备添加新的项
            ui->xuanNameBox->clear();  // 清空 QComboBox 中的所有项
            // 遍历 dataList 将每一行的 xuanName 列添加到 QComboBox 中
            for (const QList<QVariant>& row : dataList2) {
                if (!row.isEmpty()) {
                    // 假设 xuanName 是 dataList 每行的第一列（根据你的表结构调整列索引）
                    QString xuanName = row.at(1).toString();  // 获取 xuanName 的值
                    ui->xuanNameBox->addItem(xuanName);  // 将该值添加到 QComboBox
                }
            }
        }

    QString buttonStyle = R"(
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
    // 状态栏
    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    // 设置状态栏样式
    statusBar->setStyleSheet("QStatusBar { background-color: #34495e; color: white; font-size: 12px; }");

    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    menuBar->addMenu("系统(&V)");
    menuBar->addMenu("设置(&Y)");
    menuBar->addMenu("维护(&P)");
    menuBar->addMenu("工具(&T)");
    menuBar->addMenu("帮助(&Z)");

    // 设置菜单栏样式
    menuBar->setStyleSheet("QMenuBar { background-color: #2c3e50; color: white; font-size: 16px; }"
                           "QMenuBar::item { background-color: #34495e; padding: 5px; border-radius: 5px; }"
                           "QMenuBar::item:selected { background-color: #1abc9c; }");


#if 1 // 查询按钮设定

    // 创建菜单
            QMenu *queryButtonMenu = new QMenu(this);

            // 创建菜单项
            QAction *queryButtonAction1 = new QAction("全部查询", this);
            QAction *queryButtonAction2 = new QAction("按时间范围查询", this);
            QAction *queryButtonAction3 = new QAction("悬挂名称", this);
            QAction *queryButtonAction4 = new QAction("节点序列号", this);

            // 添加菜单项到菜单
            queryButtonMenu->addAction(queryButtonAction1);
            queryButtonMenu->addAction(queryButtonAction2);
            queryButtonMenu->addAction(queryButtonAction3);
            queryButtonMenu->addAction(queryButtonAction4);

            // 为菜单项连接槽函数
            connect(queryButtonAction1, &QAction::triggered, this, &HstoryList::onOption1);
            connect(queryButtonAction2, &QAction::triggered, this, &HstoryList::onOption2);
            connect(queryButtonAction3, &QAction::triggered, this, &HstoryList::onOption3);
            connect(queryButtonAction4, &QAction::triggered, this, &HstoryList::onOption4);

            // 设置按钮点击时弹出菜单
            connect(ui->queryButton, &QPushButton::clicked, this, [queryButtonMenu, this]() {
                // 弹出菜单，位置在按钮的左下角
                queryButtonMenu->exec(ui->queryButton->mapToGlobal(ui->queryButton->rect().bottomLeft()));
            });

#endif

#if 1 // 打印按钮设定
            // 创建菜单
                    QMenu *printButtonMenu = new QMenu(this);

                    // 创建菜单项
                    QAction *printButtonAction1 = new QAction("打印主记录列表(Y)", this);
                    QAction *printButtonAction2 = new QAction("打印曲线数据列表(Z)", this);

                    // 添加菜单项到菜单
                    printButtonMenu->addAction(printButtonAction1);
                    printButtonMenu->addAction(printButtonAction2);

                    // 为菜单项连接槽函数
                    connect(printButtonAction1, &QAction::triggered, this, &HstoryList::printOnOption1);
                    connect(printButtonAction2, &QAction::triggered, this, &HstoryList::printonOption2);

                    // 设置按钮点击时弹出菜单
                    connect(ui->printButton, &QPushButton::clicked, this, [printButtonMenu, this]() {
                        // 弹出菜单，位置在按钮的左下角
                        printButtonMenu->exec(ui->printButton->mapToGlobal(ui->printButton->rect().bottomLeft()));
                    });
#endif
                    connect(ui->exitButton, QPushButton::clicked, this,[=]{
                        this->close();
                    });

#if 1
    this->setFocus();
    ui->queryButton->setStyleSheet(buttonStyle);
    ui->printButton->setStyleSheet(buttonStyle);
    ui->pushButton_4->setStyleSheet(buttonStyle);
    ui->pushButton_5->setStyleSheet(buttonStyle);
    ui->exitButton->setStyleSheet(buttonStyle);
    ui->startDateEdit->setFocus();
    // 设置 QDateTimeEdit 的日历弹出功能
    ui->startDateEdit->setCalendarPopup(true);
    ui->endDateEdit->setCalendarPopup(true);
    // 设置初始日期时间为当前时间

    // 设置日期显示格式为 "yyyy-MM-dd"
    ui->startDateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->startDateEdit->setDate(QDate::currentDate());
    ui->startDateEdit->setCalendarWidget(new QCalendarWidget);
    ui->startTimeEdit->setTime(QTime::currentTime());  // 当前时间，精确到秒
    qDebug() << QTime::currentTime() << endl;

    ui->endDateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->endDateEdit->setDate(QDate::currentDate());
    QCalendarWidget *calendar = new QCalendarWidget;
    ui->endDateEdit->setCalendarWidget(calendar);
    ui->endTimeEdit->setTime(QTime::currentTime());    // 当前时间，精确到秒
    calendar->setStyleSheet("background-color: lightblue;");
    // 设置显示格式，精确到秒
    ui->startTimeEdit->setDisplayFormat("hh:mm:ss");
    ui->endTimeEdit->setDisplayFormat("hh:mm:ss");

    /**
     * @brief 当开始日期更改时，确保结束日期不小于开始日期
     */
    connect(ui->startDateEdit, &QDateEdit::dateChanged, this, [this]() {
        // 当开始日期更改时，确保结束日期不小于开始日期
        if (ui->endDateEdit->date() < ui->startDateEdit->date()) {
            ui->endDateEdit->setDate(ui->startDateEdit->date());
        }
    });

    /**
     * @brief 当开始时间更改时，确保结束时间不小于开始时间
     */
    connect(ui->startTimeEdit, &QTimeEdit::timeChanged, this, [this]() {
        // 当开始时间更改时，确保结束时间不小于开始时间
        QDateTime startDateTime(ui->startDateEdit->date(), ui->startTimeEdit->time());
        QDateTime endDateTime(ui->endDateEdit->date(), ui->endTimeEdit->time());

        if (endDateTime < startDateTime) {
            ui->endDateEdit->setDate(ui->startDateEdit->date());
            ui->endTimeEdit->setTime(ui->startTimeEdit->time());
        }
    });

    /**
     * @brief // 连接结束时间编辑控件，确保结束日期和时间不会早于开始时间
     */
    connect(ui->endDateEdit, &QDateEdit::dateChanged, this, [this]() {
        QDateTime startDateTime(ui->startDateEdit->date(), ui->startTimeEdit->time());
        QDateTime endDateTime(ui->endDateEdit->date(), ui->endTimeEdit->time());
        if (endDateTime < startDateTime) {
            ui->endDateEdit->setDate(ui->startDateEdit->date());
            ui->endTimeEdit->setTime(ui->startTimeEdit->time());
        }
    });

    connect(ui->endTimeEdit, &QTimeEdit::timeChanged, this, [this]() {
        QDateTime startDateTime(ui->startDateEdit->date(), ui->startTimeEdit->time());
        QDateTime endDateTime(ui->endDateEdit->date(), ui->endTimeEdit->time());
        if (endDateTime < startDateTime) {
            ui->endDateEdit->setDate(ui->startDateEdit->date());
            ui->endTimeEdit->setTime(ui->startTimeEdit->time());
        }
    });

#endif

    //鼠标移过时，整行背景颜色变化
//    HoveredRowItemDelegate *delegate2 = new HoveredRowItemDelegate(ui->tableWidget2);
//    ui->tableWidget2->setItemDelegate(delegate2);
//    loadTable(ui->tableWidget2);

    //隔行变色
//    ui->tableWidget2->setAlternatingRowColors(true);
//    QPalette palette;    //调色板
//    palette.setColor(QPalette::Base, QColor(255,255,255));   //基底
//    palette.setColor(QPalette::AlternateBase, QColor(250,250,250));  //交替基底
//    ui->tableWidget2->setPalette(palette);

#if 1 // 当选中某一行时将那一行中的所有数据存储
    // 连接信号，响应表格行选择变化
    connect(ui->tableWidget2, &QTableWidget::itemSelectionChanged, this, [=]() {
        rowData.clear();  // 清空全局容器中的数据
        // 获取选中的行索引
        QList<QTableWidgetItem*> selectedItems = ui->tableWidget2->selectedItems();
        if (selectedItems.isEmpty()) {
            qDebug() << "No row selected";
            return;
        }

        // 获取选中行的第一个单元格的行号
        int selectedRow = selectedItems.first()->row();
        qDebug() << "Selected row:" << selectedRow;

        // 获取选中行所有列的内容

        for (int col = 0; col < ui->tableWidget2->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableWidget2->item(selectedRow, col);
            if (item) {
                // 获取每个单元格的内容，转换为 QVariant
                rowData.append(item->text());
            }
        }
        // 当前记录的值 和数据库中主数据表的值比较返回索引
        QStringList columnNames = {"xuanName", "press_date", "operator", "inspector", "serial_number1", "pressData1",
                                   "press_result1", "force_standard1", "serial_number2", "pressData2",
                                   "press_result2", "force_standard2"};
        QString buf = HstoryList::getLastRecordId(dataBaseConn, "mainListTb", rowData, columnNames);
        qDebug() << buf << "--------" << endl;
        // 在和曲线数据列表最后一个字段比较如果相同的行变为蓝色
        // 便利ui->tableWidget2_2 最后一个字段 如果和QString buf相等 则那一条数据变红
        HstoryList::highlightMatchingRow(buf);
        // 打印选中行的所有数据
        qDebug() << "Selected row data:";
        for (const QVariant& data : rowData) {
            qDebug() << data.toString();
        }
    });

#endif
}

HstoryList::~HstoryList()
{
    delete ui;
}

/**
 * @brief 加载表格
 */
void HstoryList::loadTable(QTableWidget *tableWidget){
#if 1
    tableWidget->setColumnCount(12);

    QStringList heardList;
    heardList<<"悬挂件名称"<<"压装日期"<<"操作者"<<"检查者"<<"节点序列号1"<<"压装力值1"<<"压装结果1"<<"压装力标准1"
            <<"节点序列号2"<<"压装力值2"<<"压装结果2"<<"压装力标准2";
    tableWidget->setHorizontalHeaderLabels(heardList);

    qDebug() << "history界面" << endl;
    // 将数据库存入的数据 便利到tableWidget中

    // 查询数据库数据，注意这里查询结果中会包含 'id' 列
    mainJiLuList = HstoryList::queryTable(dataBaseConn, "./D1.db", "mainListTb");

    // 1.2 设置列的宽度，拉伸使表格充满窗口
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 1.3 获取表格的行数和列数
    int rowCount = mainJiLuList.size();
    int columnCount = (rowCount > 0) ? mainJiLuList[0].size() : 0;

    // 1.4 设置表格的行数和列数
    tableWidget->setRowCount(rowCount);
    tableWidget->setColumnCount(columnCount - 1);  // 排除 id 列，减去 1

    // 1.5 填充表格数据，跳过 'id' 列（假设 'id' 是第一列）
    for (int i = 0; i < rowCount; ++i) {
        const QList<QVariant> &row = mainJiLuList[i];
        for (int j = 1; j < columnCount; ++j) {  // 从第2列开始填充，跳过 'id' 列
            tableWidget->setItem(i, j - 1, new QTableWidgetItem(row[j].toString()));
        }
    }

    // 如果需要对表格进行额外设置或调整，可以继续进行
    // 例如，如果想要在表格中加入其他数据，继续使用 appendOneRow 或其他方式填充。

    //去除选中虚线框
    tableWidget->setFocusPolicy(Qt::NoFocus);

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//只读 不允许编辑 (整表)
//    ui->tableWidget->item(r,w)->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);  //可编辑 (单元格)

    // 设置选中行的行为
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 还可以设置选择模式为单选
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // 假设 headerView 是 QHeaderView 对象的指针，通常与 QTableWidget 或 QTreeView 绑定
    QHeaderView *headerView = tableWidget->horizontalHeader();

    // 设置第一列的大小为 100 像素
//    headerView->resizeSection(0, 300);
//    headerView->setSectionResizeMode(0, QHeaderView::Fixed); // 固定大小模式
    headerView->setMinimumHeight(48); //设置头的高度
    tableWidget->verticalHeader()->setVisible(false);//第一列序号不显示
    #endif
}

/**
 * @brief 加载表格
 */
void HstoryList::loadTable2(QTableWidget *tableWidget){
#if 1
    tableWidget->setColumnCount(3);
    QStringList heardList;
    heardList<<"压力值"<<"时期时间"<<"图表类型";
    tableWidget->setHorizontalHeaderLabels(heardList);


    qDebug() << "history界面" << endl;
    // 将数据库存入的数据 便利到tableWidget中

    // 查询数据库数据，注意这里查询结果中会包含 'id' 列
    dataList4 = HstoryList::queryTable(dataBaseConn, "./D1.db", "streetDataTb");

    // 1.2 设置列的宽度，拉伸使表格充满窗口
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 1.3 获取表格的行数和列数
    int rowCount = dataList4.size();
    int columnCount = (rowCount > 0) ? dataList4[0].size() : 0;

    // 1.4 设置表格的行数和列数
    tableWidget->setRowCount(rowCount);
    tableWidget->setColumnCount(columnCount - 1);  // 排除 id 列，减去 1

    // 1. 填充表格数据，跳过 'id' 列和最后一列
    for (int i = 0; i < rowCount; ++i) {
        const QList<QVariant> &row = dataList4[i];
        for (int j = 1; j < row.size(); ++j) {
            tableWidget->setItem(i, j - 1, new QTableWidgetItem(row[j].toString()));
        }
    }

    //去除选中虚线框
    tableWidget->setFocusPolicy(Qt::NoFocus);

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//只读 不允许编辑 (整表)
//    ui->tableWidget->item(r,w)->setFlags(Qt::NoItemFlags);         //设置不可编辑 (单元格)
//    ui->tableWidget->item(r,w)->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);  //可编辑 (单元格)

    // 设置选中行的行为
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 还可以设置选择模式为单选
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // 获取水平头视图
    QHeaderView *headerView = tableWidget->horizontalHeader();

    headerView->setMinimumHeight(48); //设置头的高度

    tableWidget->verticalHeader()->setVisible(false);//第一列序号不显示
    tableWidget->verticalHeader()->setDefaultSectionSize(48); // 设置默认行高
    tableWidget->setShowGrid(false);//设置item无边框
    #endif
}

/**
 * @brief 加载qss
 */
void HstoryList::applyStyles(QWidget *widget,QString stylesheet)
{
    widget->setStyleSheet(stylesheet); // 使用上面读取到的stylesheet
    // 遍历所有子控件并递归应用样式
    for (auto child : widget->findChildren<QWidget*>()) {
        applyStyles(child,stylesheet);
    }
}



void HstoryList::appendOneRow(QTableWidget *tableWidget,QString x1, QString x2, QString x3,
                            QString x4, QString x5, QString x6,QString x7, QString x8, QString x9,
                            QString x10, QString x11, QString x12) {
    //  获取当前行数
    int count = tableWidget->rowCount();

    // 在表格基础上行数增加一行 ，用来添加数据
   tableWidget->setRowCount(count + 1);

    QTableWidgetItem *x1Item = new QTableWidgetItem(x1);
    QTableWidgetItem *x2Item = new QTableWidgetItem(x2);
    QTableWidgetItem *x3Item = new QTableWidgetItem(x3);
    QTableWidgetItem *x4Item = new QTableWidgetItem(x4);
    QTableWidgetItem *x5Item = new QTableWidgetItem(x5);
    QTableWidgetItem *x6Item = new QTableWidgetItem(x6);
    QTableWidgetItem *x7Item = new QTableWidgetItem(x7);
    QTableWidgetItem *x8Item = new QTableWidgetItem(x8);
    QTableWidgetItem *x9Item = new QTableWidgetItem(x9);
    QTableWidgetItem *x10Item = new QTableWidgetItem(x10);
    QTableWidgetItem *x11Item = new QTableWidgetItem(x11);
    QTableWidgetItem *x12Item = new QTableWidgetItem(x12);

    // 设置居中显示
    x1Item->setTextAlignment(Qt::AlignCenter);
    x2Item->setTextAlignment(Qt::AlignCenter);
    x3Item->setTextAlignment(Qt::AlignCenter);
    x4Item->setTextAlignment(Qt::AlignCenter);
    x5Item->setTextAlignment(Qt::AlignCenter);
    x6Item->setTextAlignment(Qt::AlignCenter);
    x7Item->setTextAlignment(Qt::AlignCenter);
    x8Item->setTextAlignment(Qt::AlignCenter);
    x9Item->setTextAlignment(Qt::AlignCenter);
    x10Item->setTextAlignment(Qt::AlignCenter);
    x11Item->setTextAlignment(Qt::AlignCenter);
    x12Item->setTextAlignment(Qt::AlignCenter);

    // 第一个参数代表行 第二个代表列
    tableWidget->setItem(count, 0, x1Item);
    tableWidget->setItem(count, 1, x2Item);
    tableWidget->setItem(count, 2, x3Item);
    tableWidget->setItem(count, 3, x4Item);
    tableWidget->setItem(count, 4, x5Item);
    tableWidget->setItem(count, 5, x6Item);
    tableWidget->setItem(count, 6, x7Item);
    tableWidget->setItem(count, 7, x8Item);
    tableWidget->setItem(count, 8, x9Item);
    tableWidget->setItem(count, 9, x10Item);
    tableWidget->setItem(count, 10,x11Item);
    tableWidget->setItem(count, 11,x12Item);
}

/**
 * @brief 获取指定名称的数据库连接
 * @param dbName 数据库文件名
 * @return QSqlDatabase 返回数据库连接对象
 */
QSqlDatabase HstoryList::getDatabaseConnection(const QString &dbName) {
    QSqlDatabase database;

    // 检查是否存在默认连接
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        // 如果存在默认连接，则使用该连接
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        // 如果不存在默认连接，则创建一个新的SQLite连接
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(dbName);  // 使用传入的数据库文件名
    }


    // 打开数据库连接并检查是否成功
    if (!database.open()) {
        qDebug() << "Error: Unable to open the database!";
        qDebug() << database.lastError().text();
    } else {
        qDebug() << "数据库成功连接!!!";
    }

    return database;
}



/**
 * @brief 判断是否为指定的数据库文件，并在表不存在时创建表
 * @param db 数据库对象
 * @param dbName 数据库文件名（如 "D1.db"）
 * @param tableName 表名
 * @return true 表已存在或创建成功，且数据库名正确
 * @return false 表创建失败或数据库名不匹配
 */
bool HstoryList::ensureTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName) {
    // 检查数据库是否打开
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return false;
    }

    // 检查是否为指定的数据库文件
    if (db.databaseName() != dbName) {
        qDebug() << "Error: The database is not" << dbName << "but" << db.databaseName();
        return false;
    }

    QSqlQuery query(db);

    // 检查表是否存在
    QString checkTableSQL = QString(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='%1';"
    ).arg(tableName);

    if (query.exec(checkTableSQL)) {
        if (query.next() && query.value(0).toInt() > 0) {
            qDebug() << "Table" << tableName << "already exists in" << dbName;
            return true;
        }
    } else {
        qDebug() << "Error checking table existence:" << query.lastError();
        return false;
    }

    // 如果表不存在，则创建表
    /*

CREATE TABLE %s (
    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 主键自增
    xuanName TEXT NOT NULL,               -- 悬挂件名称
    press_date TEXT NOT NULL,             -- 压装日期
    operator TEXT NOT NULL,               -- 操作者
    inspector TEXT NOT NULL,              -- 检定者
    serial_number1 TEXT NOT NULL,          -- 书点序列号1
    pressData1 REAL NOT NULL,             -- 压装力值1
    press_result1 TEXT NOT NULL,           -- 压装结果1
    force_standard1 TEXT NOT NULL          -- 压装力标准1
    serial_number2 TEXT NOT NULL,          -- 书点序列号1
    pressData2 REAL NOT NULL,             -- 压装力值1
    press_result2 TEXT NOT NULL,           -- 压装结果1
    force_standard2 TEXT NOT NULL          -- 压装力标准1
);
*/
    QString createSQL = QString(
                "CREATE TABLE %1 ("
                "    id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "    xuanName TEXT NOT NULL, "
                "    press_date DATE NOT NULL, "
                "    operator TEXT NOT NULL, "
                "    inspector TEXT NOT NULL, "
                "    serial_number1 TEXT NOT NULL, "
                "    pressData1 TEXT NOT NULL, "
                "    press_result1 TEXT NOT NULL, "
                "    force_standard1 TEXT NOT NULL, "
                "    serial_number2 TEXT NOT NULL, "
                "    pressData2 TEXT NOT NULL, "
                "    press_result2 TEXT NOT NULL, "
                "    force_standard2 TEXT NOT NULL"
                ");"
    ).arg(tableName);

    query.prepare(createSQL);
    if (!query.exec()) {
        qDebug() << "Error: Failed to create table." << query.lastError();
        return false;
    }

    qDebug() << "Table" << tableName << "created successfully in" << dbName;
    return true;
}

/**
 * @brief 在指定数据库和表中插入数据
 * @param db 数据库对象
 * @param dbName 数据库名称
 * @param tableName 表名称
 * @param values 要插入的数据（QVariant列表）
 * @return true 插入成功
 * @return false 插入失败
 */
bool HstoryList::insertIntoTable(QSqlDatabase &db, const QString &dbName, const QString &tableName, const QList<QString> &columns, const QList<QVariant> &values) {
    if (!db.isOpen() || db.databaseName() != dbName) return false;
    if (columns.size() != values.size()) return false;

    QSqlQuery query(db);

    // 使用循环生成与列数匹配的占位符
    QStringList placeholders;
    for (int i = 0; i < columns.size(); ++i) {
        placeholders << "?";
    }

    QString insertSQL = QString("INSERT INTO %1 (%2) VALUES (%3);")
                        .arg(tableName)
                        .arg(columns.join(", "))
                        .arg(placeholders.join(", "));  // 拼接占位符

    query.prepare(insertSQL);
    for (int i = 0; i < values.size(); ++i) {
        query.bindValue(i, values[i]);
    }

    return query.exec();
}


/**
 * @brief 将从库中拿到的信息存入QList<QVariantList>
 */
QList<QList<QVariant>> HstoryList::queryTable(QSqlDatabase &db, const QString &dbName, const QString &tableName,
                                              const QStringList &columns,
                                              const QString &condition) {
    QList<QList<QVariant>> results;

    // 检查数据库连接是否有效
    if (!db.isOpen() || db.databaseName() != dbName) return results;

    QSqlQuery query(db);

    // 构造 SQL 查询语句
    QString columnStr = columns.join(", ");
    QString selectSQL = QString("SELECT %1 FROM %2").arg(columnStr).arg(tableName);
    if (!condition.isEmpty()) {
        selectSQL += QString(" WHERE %1").arg(condition);
    }

    // 执行查询
    if (!query.exec(selectSQL)) {
        qDebug() << "查询失败:" << query.lastError();
        return results;
    }

    // 遍历查询结果
    while (query.next()) {
        QList<QVariant> row;
        for (int i = 0; i < query.record().count(); ++i) {
            row.append(query.value(i));  // 添加每列的数据
        }
        results.append(row);  // 添加每行数据
    }

    return results;
}



#if 1
/**
 * @brief 按照时间区间查询数据库
 */
QList<QList<QVariant>> HstoryList::queryTableDate(QSqlDatabase &db, const QString &dbName, const QString &tableName,
                                               const QString &startDateTime, const QString &endDateTime,const QStringList &columns) {
    QList<QList<QVariant>> results;

    // 检查数据库连接是否有效
    if (!db.isOpen() || db.databaseName() != dbName) {
        qDebug() << "数据库连接无效!";
        return results;
    }
    QSqlQuery query(db);

    // 构造 SQL 查询语句
    QString columnStr = columns.join(", ");
    QString selectSQL = QString("SELECT %1 FROM %2").arg(columnStr).arg(tableName);

    // 生成时间范围的条件 (假设时间字段名为 "press_date")
    QString condition;
    if (!startDateTime.isEmpty() && !endDateTime.isEmpty()) {
//        qDebug() << "enter1111" << endl;
        // 过滤时间大于开始时间且小于结束时间
        condition = QString("WHERE press_date >= '%1' AND press_date <= '%2'").arg(startDateTime).arg(endDateTime);
        qDebug() << condition << endl;

    } else if (!startDateTime.isEmpty()) {
                qDebug() << "enter1111" << endl;
        // 过滤时间大于开始时间
        condition = QString("WHERE press_date >= '%1'").arg(startDateTime);
                qDebug() << condition << "开始---------------" << endl;

    } else if (!endDateTime.isEmpty()) {
        // 过滤时间小于结束时间
        condition = QString("WHERE press_date <= '%1'").arg(endDateTime);
        qDebug() << condition << "jieshu---------------" << endl;
    }

    if (!condition.isEmpty()) {
        selectSQL += " " + condition;
    }

    // 执行查询
    if (!query.exec(selectSQL)) {
        qDebug() << "查询失败:" << query.lastError();
        return results;
    }

    // 遍历查询结果并存储到 results 中
    while (query.next()) {
        QList<QVariant> row;
        for (int i = 0; i < query.record().count(); ++i) {
            row.append(query.value(i));  // 添加每列的数据
        }
        results.append(row);  // 添加每行数据
    }
//   qDebug() << results << endl;

    return results;
}
#endif

/**
 * @brief 按时间查询槽函数
 */
// 按照时间查询
void HstoryList::onOption2() {
    qDebug("选项2被选择");
    ui->tableWidget2->setColumnCount(12);
    QStringList heardList;
    heardList<<"悬挂件名称"<<"压装日期"<<"操作者"<<"检查者"<<"节点序列号1"<<"压装力值1"<<"压装结果1"<<"压装力标准1"
            <<"节点序列号2"<<"压装力值2"<<"压装结果2"<<"压装力标准2";
    ui->tableWidget2->setHorizontalHeaderLabels(heardList);

    // 获取时间范围
    QString startDateTime = ui->startDateEdit->text() + " " + ui->startTimeEdit->text();
    QString endDateTime = ui->endDateEdit->text() + " " + ui->endTimeEdit->text();
//    数据插入容器成功
    dateFind = HstoryList::queryTableDate(dataBaseConn, "./D1.db", "mainListTb",startDateTime,endDateTime);
    ui->tableWidget2->clearContents();
#if 1
    // 1.2 设置列的宽度，拉伸使表格充满窗口
    ui->tableWidget2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 1.3 获取表格的行数和列数
    int rowCount = dateFind.size();
    int columnCount = (rowCount > 0) ? dateFind[0].size() : 0;
    // 1.4 设置表格的行数和列数
    ui->tableWidget2->setRowCount(rowCount);
    ui->tableWidget2->setColumnCount(columnCount - 1);  // 排除 id 列，减去 1
    // 1.5 填充表格数据，跳过 'id' 列（假设 'id' 是第一列）
    for (int i = 0; i < rowCount; ++i) {
        const QList<QVariant> &row = dateFind[i];
        for (int j = 1; j < columnCount; ++j) {  // 从第2列开始填充，跳过 'id' 列
            ui->tableWidget2->setItem(i, j - 1, new QTableWidgetItem(row[j].toString()));
        }
    }
    // 时间查询时将容器存入主容器中因为打印pdf处调用的是主容器
    mainJiLuList.clear();
    mainJiLuList.append(dateFind);

//    // 1.6 末尾行添加统计行，显示数量
//    ui->tableWidget2->setRowCount(rowCount + 1);  // 增加一行用于显示统计信息
//    ui->tableWidget2->setItem(rowCount, 0, new QTableWidgetItem("总计"));
//    ui->tableWidget2->setItem(rowCount, 1, new QTableWidgetItem(QString::number(rowCount)));
//    // 合并第二列到最后一列，形成一个跨度
//    int columnCount1 = ui->tableWidget2->columnCount();
//    ui->tableWidget2->setSpan(rowCount, 1, 1, columnCount1 - 1);  // 合并第二列到最后一列

#endif

#if 1 // 按时间查询曲线数据
       dateFind2 = HstoryList::queryTableDate(dataBaseConn, "./D1.db", "streetDataTb",startDateTime,endDateTime);
       ui->tableWidget2_2->clearContents();
      // 处理查询到的数据
       ui->tableWidget2_2->setColumnCount(3);
       QStringList heardList2;
       heardList2<<"压装力值"<<"压装日期"<<"图表类型";
       ui->tableWidget2_2->setHorizontalHeaderLabels(heardList2);
       // 1.2 设置列的宽度，拉伸使表格充满窗口
       ui->tableWidget2_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
       // 1.3 获取表格的行数和列数
       int rowCount2 = dateFind2.size();
       int columnCount2 = (rowCount2 > 0) ? dateFind2[0].size() : 0;
       // 1.4 设置表格的行数和列数
       ui->tableWidget2_2->setRowCount(rowCount2);
       ui->tableWidget2_2->setColumnCount(columnCount2 - 1);  // 排除 id 列，减去 1
       // 1.5 填充表格数据，跳过 'id' 列（假设 'id' 是第一列）
       for (int i = 0; i < rowCount2; ++i) {
           const QList<QVariant> &row = dateFind2[i];
           for (int j = 1; j < columnCount2; ++j) {  // 从第2列开始填充，跳过 'id' 列
               ui->tableWidget2_2->setItem(i, j - 1, new QTableWidgetItem(row[j].toString()));
           }
       }
#endif
       //去除选中虚线框
       ui->tableWidget2->setFocusPolicy(Qt::NoFocus);
       ui->tableWidget2->setEditTriggers(QAbstractItemView::NoEditTriggers);//只读 不允许编辑 (整表)
       ui->tableWidget2_2->setFocusPolicy(Qt::NoFocus);
       ui->tableWidget2_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//只读 不允许编辑 (整表)
       // 设置选中行的行为
       ui->tableWidget2->setSelectionBehavior(QAbstractItemView::SelectRows);
       ui->tableWidget2_2->setSelectionBehavior(QAbstractItemView::SelectRows);
       // 还可以设置选择模式为单选
       ui->tableWidget2->setSelectionMode(QAbstractItemView::SingleSelection);
       ui->tableWidget2_2->setSelectionMode(QAbstractItemView::SingleSelection);

    qDebug() << startDateTime << "--" << endDateTime << endl;
}


// 菜单项对应的槽函数
    // 全部查询
    void HstoryList::onOption1() {
        qDebug("选项1被选择");
        HstoryList::loadTable(ui->tableWidget2);
        HstoryList::loadTable2(ui->tableWidget2_2);
    }

    // 悬挂名称
    void HstoryList::onOption3() {
        //如果当前QLineEdit中的数据和数据库xuanName字段名称相等将数据库中的数据
        // 插入的 table中
        qDebug("选项3被选择");
    }
    // 节点序列号
    void HstoryList::onOption4() {
        // 如果当前QLiineEdit中的节点序列号和数据库中的Serial_number1相等
        // 将数据库中的数据插入到Table中
        qDebug("选项4被选择");
    }
    // 打印主记录列表(Y)
    void HstoryList::printOnOption1() {
        qDebug("打印触发1");
         HstoryList::exportPdf(); // 主记录存成pdf
         HstoryList::filePrintPreview2();
    }
    //打印曲线数据列表(Z)
    void HstoryList::printonOption2() {
        qDebug("打印触发2");
        HstoryList::filePrintPreview();
    }


    // 打印预览槽函数
    void HstoryList::filePrintPreview()
    {


        // 53%
    #if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
        QPrinter printer(QPrinter::HighResolution);
        QPrintPreviewDialog preview(&printer, this);
        preview.resize(2400,1600);

        connect(&preview, &QPrintPreviewDialog::paintRequested, this, &HstoryList::printPreview);
        preview.exec();
    #endif
    }
    void HstoryList::printPreview(QPrinter *printer)
    {
    #ifdef QT_NO_PRINTER
        Q_UNUSED(printer);
    #else
        // 设置打印机页面为 A4 横向
        printer->setPageSize(QPrinter::A4);
        printer->setOrientation(QPrinter::Landscape);  // 设置为横向

        QString filePath = QFileDialog::getOpenFileName(this, "选择PDF文件", "", "PDF Files (*.pdf)");
        if (filePath.isEmpty()) {
            return;
        }

        printer->setResolution(300);

        // 创建 Poppler 文档
        Poppler::Document *pdfDocument = Poppler::Document::load(filePath);
        if (!pdfDocument) {
            qWarning() << "Failed to load PDF:" << filePath;
            return;
        }

        // 打印 PDF 的第一页
        int pageNumber = 0;  // 0-based, 第一页
        Poppler::Page *page = pdfDocument->page(pageNumber);
        if (!page) {
            qWarning() << "Failed to load page" << pageNumber;
            delete pdfDocument;
            return;
        }

        // 获取打印页面的大小（横向 A4）
        QSize pageSize = printer->pageRect().size(); // 获取打印机页面的尺寸

        // 设置渲染为高分辨率图像，600 DPI 或更高
        QImage image = page->renderToImage(600, 600);  // 使用 600 DPI 渲染图像
        if (image.isNull()) {
            qWarning() << "Failed to render page to image";
            delete pdfDocument;
            return;
        }

        // 创建 QPainter 来绘制图像到打印机
        QPainter painter(printer);
        if (!painter.isActive()) {  // 确保 QPainter 初始化成功
            qWarning() << "Failed to initialize QPainter";
            delete pdfDocument;
            return;
        }

        // 保存当前 QPainter 状态
        painter.save();

        // 设置打印页面上的目标区域
        QRect targetRect(0, 0, pageSize.width(), pageSize.height());  // 使用整个页面宽高作为目标区域
        painter.translate(targetRect.topLeft());

        // 计算图像缩放比例（不保持比例，强制图像充满页面）
        qreal scaleX = targetRect.width() / static_cast<qreal>(image.width());
        qreal scaleY = targetRect.height() / static_cast<qreal>(image.height());

        // 强制按页面大小缩放图像，使用最大的比例
        qreal scale = qMax(scaleX, scaleY);  // 使用最大的缩放比例，确保图像充满页面

        // 缩放绘制
        painter.scale(scale, scale);

        // 将 PDF 图像绘制到打印机上
        painter.drawImage(0, 0, image);

        // 恢复 QPainter 状态
        painter.restore();

        // 清理
        delete pdfDocument;

    #endif
    }

 #if 1

    // 打印预览槽函数2
    void HstoryList::filePrintPreview2()
    {


        // 53%
    #if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
        QPrinter printer(QPrinter::HighResolution);
        QPrintPreviewDialog preview(&printer, this);
        preview.resize(2400,1600);

        connect(&preview, &QPrintPreviewDialog::paintRequested, this, &HstoryList::printPreview2);
        preview.exec();
    #endif
    }
    void HstoryList::printPreview2(QPrinter *printer)
    {
    #ifdef QT_NO_PRINTER
        Q_UNUSED(printer);
    #else
        // 设置打印机页面为 A4 横向
        printer->setPageSize(QPrinter::A4);
        printer->setOrientation(QPrinter::Landscape);  // 设置为横向

        QString filePath = QFileDialog::getOpenFileName(this, "选择PDF文件", "", "PDF Files (*.pdf)");
        if (filePath.isEmpty()) {
            return;
        }

        printer->setResolution(300);

        // 创建 Poppler 文档
        Poppler::Document *pdfDocument = Poppler::Document::load(filePath);
        if (!pdfDocument) {
            qWarning() << "Failed to load PDF:" << filePath;
            return;
        }

        // 打印 PDF 的第一页
        int pageNumber = 0;  // 0-based, 第一页
        Poppler::Page *page = pdfDocument->page(pageNumber);
        if (!page) {
            qWarning() << "Failed to load page" << pageNumber;
            delete pdfDocument;
            return;
        }

        // 获取打印页面的大小（横向 A4）
        QSize pageSize = printer->pageRect().size(); // 获取打印机页面的尺寸

        // 设置渲染为高分辨率图像，600 DPI 或更高
        QImage image = page->renderToImage(600, 600);  // 使用 600 DPI 渲染图像
        if (image.isNull()) {
            qWarning() << "Failed to render page to image";
            delete pdfDocument;
            return;
        }

        // 创建 QPainter 来绘制图像到打印机
        QPainter painter(printer);
        if (!painter.isActive()) {  // 确保 QPainter 初始化成功
            qWarning() << "Failed to initialize QPainter";
            delete pdfDocument;
            return;
        }

        // 保存当前 QPainter 状态
        painter.save();

        // 设置打印页面上的目标区域
        QRect targetRect(0, 0, pageSize.width(), pageSize.height());  // 使用整个页面宽高作为目标区域
        painter.translate(targetRect.topLeft());

        // 计算图像缩放比例（不保持比例，强制图像充满页面）
        qreal scaleX = targetRect.width() / static_cast<qreal>(image.width());
        qreal scaleY = targetRect.height() / static_cast<qreal>(image.height());

        // 强制按页面大小缩放图像，使用最大的比例
        qreal scale = qMax(scaleX, scaleY);  // 使用最大的缩放比例，确保图像充满页面

        // 缩放绘制
        painter.scale(scale, scale);

        // 将 PDF 图像绘制到打印机上
        painter.drawImage(0, 0, image);

        // 恢复 QPainter 状态
        painter.restore();

        // 清理
        delete pdfDocument;

    #endif
    }

#endif


    /**
     * @brief 判断是否为指定的数据库文件，查询表中所有数据存入容器中
     * @param db 数据库对象
     * @param dbName 数据库文件名（如 "D1.db"）
     * @param tableName 表名
     * @return true 查询存入容器成功
     * @return false 查询存入容器失败
     */
    bool HstoryList::queryAllDataFromTableXuan(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList) {
            dataList.clear();
        // 检查数据库是否打开
        if (!db.isOpen()) {
            qDebug() << "Error: Database is not open.";
            return false;
        }

        QSqlQuery query(db);

        // 构建查询所有数据的 SQL 语句
        QString selectSQL = QString("SELECT * FROM %1;").arg(tableName);

        if (!query.exec(selectSQL)) {
            qDebug() << "Error executing query:" << query.lastError();
            return false;
        }

        // 获取查询结果的字段信息
           QSqlRecord record = query.record();
           int fieldCount = record.count();  // 获取字段数量

           // 处理查询结果
           while (query.next()) {
               QList<QVariant> rowData;  // 存储当前行的所有列数据

               // 遍历所有列
               for (int i = 0; i < fieldCount; ++i) {
                   QVariant fieldValue = query.value(i);  // 获取列值
                   rowData.append(fieldValue);  // 将列值存入 QList
               }

               // 将当前行数据插入到 dataList 中
               dataList.append(rowData);
           }

        qDebug() << "Query executed successfully, retrieved" << dataList.size() << "rows of data.";
        return true;
    }


    /**
     * @brief 判导出主记录表
     */
    void HstoryList::exportPdf()
    {
        // 设置默认保存路径
        QString defaultPath = QCoreApplication::applicationDirPath() + "/mainJiLu.pdf";  // 默认保存路径为应用程序目录下

        // 如果文件没有后缀，则添加 .pdf 后缀
        if (QFileInfo(defaultPath).suffix().isEmpty()) {
            defaultPath.append(".pdf");
            qDebug() << defaultPath << endl;
        }

        // 创建 QPdfWriter 对象并设置 PDF 输出路径
        QPdfWriter pdfWriter(defaultPath);
        pdfWriter.setPageSize(QPagedPaintDevice::A4);
        pdfWriter.setResolution(QPrinter::ScreenResolution);

        // 创建 QTextDocument 来保存 HTML 内容
        QTextDocument textDocument;

        // 清空 HTML 内容，准备添加新内容
        QString m_html;
        m_html.append("<html><head><style>"
                      "th, td { border: 6px solid black; padding: 8px; text-align: center; }"
                      "</style></head><body>");

        // 标题
        m_html.append("<h2 style='text-align: center;'>主记录列表</h2><br/>");
        // 添加表格内容
        m_html.append("<h3 style='text-align: left;'>统计时间:    "
                      + ui->startDateEdit->text() + " => "
                      + ui->endDateEdit->text() + " 23:59:59</h3>");
        m_html.append("<table border='0.5' cellspacing='0' cellpadding='3' width='100%'>");
        // 添加第一行的表头
        m_html.append("<tr>"
                      "<th>悬挂名称</th>"
                      "<th>压装日期</th>"
                      "<th>操作者</th>"
                      "<th>检查者</th>"
                      "<th>节点序列号1</th>"
                      "<th>压装值力1</th>"
                      "<th>压装结果1</th>"
                      "<th>压装力标准1</th>"
                      "<th>节点序列号2</th>"
                      "<th>压装值力2</th>"
                      "<th>压装结果2</th>"
                      "<th>压装力标准2</th>"
                      "</tr>");

        // 逐行添加记录到表格
        for (const auto& recordList : mainJiLuList) {
            m_html.append("<tr>");

            // 跳过第一个元素
            for (int i = 1; i < recordList.size(); ++i) {
                m_html.append("<td>" + recordList[i].toString() + "</td>");
            }

            m_html.append("</tr>");
        }
        // 在最后添加“数据条数”行
        m_html.append("<tr><td colspan='11' style='text-align: right;'><strong>数据条数: " + QString::number(mainJiLuList.size()) + "</strong></td></tr>");
        qDebug() << "mainJiLuList size: " << mainJiLuList.size();

        // 结束表格
        m_html.append("</table><br />");

        // 结束 HTML
        m_html.append("</body></html>");

        // 将 HTML 内容设置到 QTextDocument
        textDocument.setHtml(m_html);

        // 使用 QPdfWriter 打印 PDF
        textDocument.print(&pdfWriter);

        // 关闭文件并开始新的一页
        pdfWriter.newPage();  // 如果需要新页面，使用 newPage
    }

    /**
     * @brief 查找主记录表主键id
     */
    QString HstoryList::getLastRecordId(QSqlDatabase &db, const QString &tableName, const QList<QVariant> &rowData, const QStringList &columnNames) {
        // 检查数据库是否打开
        if (!db.isOpen()) {
            qDebug() << "Error: Database is not open.";
            return QString();
        }

        // 确保 rowData 和 columnNames 数量一致
        if (rowData.size() != columnNames.size()) {
            qDebug() << "Error: Parameter count mismatch between rowData and columnNames.";
            return QString();
        }

        // 构建查询条件
        QString condition = "";
        for (int i = 0; i < rowData.size(); ++i) {
            if (i > 0) {
                condition += " AND ";  // 添加 AND 连接条件
            }
            condition += QString("%1 = ?").arg(columnNames[i]);  // 用 ? 作为占位符
        }

        // 构建 SQL 查询语句，查找符合条件的记录
        QString queryStr = QString(
            "SELECT id FROM %1 WHERE %2 ORDER BY id DESC LIMIT 1;"
        ).arg(tableName).arg(condition);

        qDebug() << "Generated Query: " << queryStr;  // 打印查询语句，检查是否正确

        // 创建 SQL 查询对象
        QSqlQuery query(db);
        query.prepare(queryStr);

        // 绑定参数
        for (int i = 0; i < rowData.size(); ++i) {
            query.bindValue(i, rowData[i].toString());  // 绑定每一列的值（确保是字符串类型）
            qDebug() << "Binding value: " << rowData[i].toString() << " to " << columnNames[i];  // 打印绑定的值和列名
        }

        // 执行查询
        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query" << query.lastError();
            return QString();
        }

        // 获取查询结果
        if (query.next()) {
            return query.value(0).toString();  // 返回 id
        } else {
            qDebug() << "Error: No records found in table" << tableName;
            return QString();
        }
    }

    /**
     * @brief 单击主记录 曲线数据高亮
     */
    void HstoryList::highlightMatchingRow(const QString &buf) {
        // 获取表格的行数
        int rowCount = ui->tableWidget2_2->rowCount();

        // 获取最后一列的索引（假设最后一列是我们要检查的列）
        int lastColumnIndex = ui->tableWidget2_2->columnCount() - 1;

        // 清除所有高亮
        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < ui->tableWidget2_2->columnCount(); ++col) {
                QTableWidgetItem *cellItem = ui->tableWidget2_2->item(row, col);
                if (cellItem) {
                    cellItem->setBackground(QBrush(Qt::white));  // 恢复默认背景色
                }
            }
        }

        // 遍历每一行
        for (int row = 0; row < rowCount; ++row) {
            // 获取最后一列单元格的内容
            QTableWidgetItem *item = ui->tableWidget2_2->item(row, lastColumnIndex);

            // 如果该单元格存在且其文本与 buf 相等
            if (item && item->text() == buf) {
                // 设置整行背景色为红色
                for (int col = 0; col < ui->tableWidget2_2->columnCount(); ++col) {
                    QTableWidgetItem *cellItem = ui->tableWidget2_2->item(row, col);
                    if (cellItem) {
                        cellItem->setBackground(QBrush(QColor("#DCE2F1")));

                    }
                }
            }
        }
    }
