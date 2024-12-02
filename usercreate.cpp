#include "usercreate.h"
#include "ui_usercreate.h"

UserCreate::UserCreate(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserCreate)
{
    ui->setupUi(this);
    database = UserCreate::getDatabaseConnection("../qtModBus/D1.db"); //数据库连接
    //去除选中虚线框
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//只读 不允许编辑 (整表)
    // 设置选中行的行为
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 还可以设置选择模式为单选
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // 用于创建操作者表
    connect(ui->liJiCreateBtn, QPushButton::clicked, this, [=]{
        qDebug() << "操作者" << endl;
        QString caoZuoLine = ui->caoZuoLine->text();
        // 获取当前日期和时间
        QDateTime currentDateTime = QDateTime::currentDateTime();

        // 格式化为 "yyyy-MM-dd HH:mm:ss" 形式（包括秒）
        QString creDataTime = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");
        bool flags = UserCreate::ensureTableInDatabase(database, "../qtModBus/D1.db", "operatorTb");
        if(flags)
            qDebug() << "operatorTb 创建成功" << endl;
        flags = UserCreate::insertDataIntoTable(database, "operatorTb", caoZuoLine, creDataTime);
    });
    // 用于创建检查者表
    connect(ui->liJiCreateBtn_2,QPushButton::clicked, this, [=]{
        qDebug() << "检查者" << endl;
        QString jianChaLine = ui->jianChaLine->text();
        // 获取当前日期和时间
        QDateTime currentDateTime = QDateTime::currentDateTime();

        // 格式化为 "yyyy-MM-dd HH:mm:ss" 形式（包括秒）
        QString creDataTime = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");
        bool flags = UserCreate::ensureTableInDatabase(database, "../qtModBus/D1.db", "inspectorTb");
        if(flags)
            qDebug() << "inspectorTb 创建成功" << endl;
        flags = UserCreate::insertDataIntoTable(database, "inspectorTb", jianChaLine, creDataTime);
    });
    // 查询按钮里面包含菜单，菜单里包含检查者、创建者
#if 1
    // 创建菜单
            QMenu *queryButtonMenu = new QMenu(this);

            // 创建菜单项
            QAction *queryButtonAction1 = new QAction("操作者", this);
            QAction *queryButtonAction2 = new QAction("检查者", this);

            // 添加菜单项到菜单
            queryButtonMenu->addAction(queryButtonAction1);
            queryButtonMenu->addAction(queryButtonAction2);

            // 为菜单项连接槽函数
            connect(queryButtonAction1, &QAction::triggered, this, &UserCreate::onOption1);
            connect(queryButtonAction2, &QAction::triggered, this, &UserCreate::onOption2);

            // 设置按钮点击时弹出菜单
            connect(ui->findBtn, &QPushButton::clicked, this, [queryButtonMenu, this]() {
                // 弹出菜单，位置在按钮的左下角
                queryButtonMenu->exec(ui->findBtn->mapToGlobal(ui->findBtn->rect().bottomLeft()));
            });

#endif
}

UserCreate::~UserCreate()
{
    delete ui;
}

/**
 * @brief 获取指定名称的数据库连接
 * @param dbName 数据库文件名
 * @return QSqlDatabase 返回数据库连接对象
 */
QSqlDatabase UserCreate::getDatabaseConnection(const QString &dbName) {
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
bool UserCreate::ensureTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName) {
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
    QString createSQL = QString(
        "CREATE TABLE %1 ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "    u_name TEXT NOT NULL, "
        "    up_date DATE NOT NULL"
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
 * @brief 判断是否为指定的数据库文件，向表中插入数据
 * @param db 数据库对象
 * @param dbName 数据库文件名（如 "D1.db"）
 * @param tableName 表名
 * @return false 表插入失败或数据库名不匹配
 */
bool UserCreate::insertDataIntoTable(QSqlDatabase &db, const QString &tableName, const QString &userName, const QString &upDate) {
    // 检查数据库是否打开
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return false;
    }

    QSqlQuery query(db);

    // 插入数据的 SQL 语句
    QString insertSQL = QString(
        "INSERT INTO %1 (u_name, up_date) "
        "VALUES (:userName, :upDate);"
    ).arg(tableName);

    query.prepare(insertSQL);
    query.bindValue(":userName", userName);
    query.bindValue(":upDate", upDate);

    if (!query.exec()) {
        qDebug() << "Error inserting data:" << query.lastError();
        return false;
    }

    qDebug() << "Data inserted successfully into" << tableName;
    return true;
}

/**
 * @brief 判断是否为指定的数据库文件，查询表中所有数据存入容器中
 * @param db 数据库对象
 * @param dbName 数据库文件名（如 "D1.db"）
 * @param tableName 表名
 * @return true 查询存入容器成功
 * @return false 查询存入容器失败
 */
bool UserCreate::queryAllDataFromTable(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList) {
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

void UserCreate::onOption1(){
    qDebug() << "操作者" << endl;
    // 假设 dataList 是你的查询结果
    bool flags = UserCreate::queryAllDataFromTable(database, "operatorTb", dataList);
    if (flags)
        qDebug() << "查询成功" << endl;

    // 清空之前的数据
    ui->tableWidget->clearContents();  // 清空表格内容
    ui->tableWidget->setRowCount(0);   // 删除所有行
    // 设置表头，假设你的表头是根据 dataList 的第一行字段数来设置的
    QStringList headerLabels;
    headerLabels << "操作者" << "创建时间";  // 自定义表头字段名
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);  // 设置表头

    // 设置表格的列数，减去 id 列（假设你不显示 id）
    ui->tableWidget->setColumnCount(dataList.isEmpty() ? 0 : dataList.first().size() - 1);
    // 设置表格的行数
    ui->tableWidget->setRowCount(dataList.size());  // 设置表格行数
    ui->tableWidget->setColumnWidth(0, 200);  // 第 1 列宽度为 150
    ui->tableWidget->setColumnWidth(1, 950);  // 第 2 列宽度为 200
    // 遍历 dataList，将每行数据插入表格（跳过 id 列）
    for (int row = 0; row < dataList.size(); ++row) {
        const QList<QVariant>& rowData = dataList.at(row);

        // 遍历每列并将数据插入到相应单元格
        int colIndex = 0;  // 用来控制列索引
        for (int col = 0; col < rowData.size(); ++col) {
            if (col == 0) {  // 假设 id 在第一列，不插入 id 列
                continue;
            }

            QString cellValue = rowData.at(col).toString();  // 获取每列的字段值
            ui->tableWidget->setItem(row, colIndex, new QTableWidgetItem(cellValue));  // 设置表格单元格的值
            colIndex++;  // 只有在插入数据时才递增 colIndex
        }
    }
}

void UserCreate::onOption2(){
    qDebug() << "检查者" << endl;
    // 假设 dataList 是你的查询结果
    bool flags = UserCreate::queryAllDataFromTable(database, "inspectorTb", dataList);
    if (flags)
        qDebug() << "查询成功" << endl;

    // 清空之前的数据
    ui->tableWidget->clearContents();  // 清空表格内容
    ui->tableWidget->setRowCount(0);   // 删除所有行
    // 设置表头，假设你的表头是根据 dataList 的第一行字段数来设置的
    QStringList headerLabels;
    headerLabels << "检查者" << "创建时间";  // 自定义表头字段名
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);  // 设置表头
    // 设置表格的列数，减去 id 列（假设你不显示 id）
    ui->tableWidget->setColumnCount(dataList.isEmpty() ? 0 : dataList.first().size() - 1);
    // 设置表格的行数
    ui->tableWidget->setRowCount(dataList.size());  // 设置表格行数
    ui->tableWidget->setColumnWidth(0, 200);  // 第 1 列宽度为 150
    ui->tableWidget->setColumnWidth(1, 950);  // 第 2 列宽度为 200
    // 遍历 dataList，将每行数据插入表格（跳过 id 列）
    for (int row = 0; row < dataList.size(); ++row) {
        const QList<QVariant>& rowData = dataList.at(row);

        // 遍历每列并将数据插入到相应单元格
        int colIndex = 0;  // 用来控制列索引
        for (int col = 0; col < rowData.size(); ++col) {
            if (col == 0) {  // 假设 id 在第一列，不插入 id 列
                continue;
            }

            QString cellValue = rowData.at(col).toString();  // 获取每列的字段值
            ui->tableWidget->setItem(row, colIndex, new QTableWidgetItem(cellValue));  // 设置表格单元格的值
            colIndex++;  // 只有在插入数据时才递增 colIndex
        }
    }
}
