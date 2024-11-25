#include "hstorylist.h"
#include "ui_hstorylist.h"

HstoryList::HstoryList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HstoryList)
{
    ui->setupUi(this);
    setWindowTitle("悬挂件压装力测试系统");
    resize(1200, 800);
    QFile file(":/Table.qss");
    file.open(QFile::ReadOnly);
    QString qss=file.readAll();
    file.close();

    applyStyles(this,qss);

    //鼠标移过时，整行背景颜色变化
    HoveredRowItemDelegate *delegate2 = new HoveredRowItemDelegate(ui->tableWidget2);
    ui->tableWidget2->setItemDelegate(delegate2);


    loadTable(ui->tableWidget2);

    //隔行变色
    ui->tableWidget2->setAlternatingRowColors(true);
    QPalette palette;    //调色板
    palette.setColor(QPalette::Base, QColor(255,255,255));   //基底
    palette.setColor(QPalette::AlternateBase, QColor(250,250,250));  //交替基底
    ui->tableWidget2->setPalette(palette);
}

HstoryList::~HstoryList()
{
    delete ui;
}
void HstoryList::loadTable(QTableWidget *tableWidget){
    tableWidget->setColumnCount(12);
    QStringList heardList;
    heardList<<"悬挂件名称"<<"压装日期"<<"操作者"<<"检查者"<<"节点序列号1"<<"压装力值1"<<"压装结果1"<<"压装力标准1"
            <<"节点序列号2"<<"压装力值2"<<"压装结果2"<<"压装力标准2";
    tableWidget->setHorizontalHeaderLabels(heardList);
    QSqlDatabase dataBaseConn;
    dataBaseConn = HstoryList::getDatabaseConnection("../qtModBus/D1.db");
    qDebug() << "history界面" << endl;
    // 将数据库存入的数据 便利到tableWidget中
#if 1
    // 查询数据库数据，注意这里查询结果中会包含 'id' 列
    QList<QList<QVariant>> mainJiLuList = HstoryList::queryTable(dataBaseConn, "../qtModBus/D1.db", "mainListTb");

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

#endif

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

    // 设置第二列的宽度为固定值
//    headerView->setSectionResizeMode(1, QHeaderView::Fixed);
//    headerView->resizeSection(1, 130);
//    // 设置第三列的大小为200像素
//    headerView->setSectionResizeMode(2, QHeaderView::Fixed); // 设置列的大小模式为固定
//    headerView->resizeSection(2, 180); // 设置第三列的具体宽度

    headerView->setMinimumHeight(48); //设置头的高度

    tableWidget->verticalHeader()->setVisible(false);//第一列序号不显示
    tableWidget->verticalHeader()->setDefaultSectionSize(48); // 设置默认行高
    tableWidget->setShowGrid(false);//设置item无边框
}

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

// 将从库中拿到的信息存入QList<QVariantList>
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

