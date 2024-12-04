#include "usercreate.h"
#include "ui_usercreate.h"

int flagsBtn = -1;
int index3 = -1;
UserCreate::UserCreate(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserCreate)
{
    ui->setupUi(this);
    database = UserCreate::getDatabaseConnection("./D1.db"); //数据库连接
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
        bool flags = UserCreate::ensureTableInDatabase(database, "./D1.db", "operatorTb");
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
        bool flags = UserCreate::ensureTableInDatabase(database, "./D1.db", "inspectorTb");
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
#if 1 // 点击tableWidget存储此条消息
#if 1 // 当选中某一行时将那一行中的所有数据存储
    // 连接信号，响应表格行选择变化
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, [=]() {
        rowData.clear();  // 清空全局容器中的数据
        // 获取选中的行索引
        QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
        if (selectedItems.isEmpty()) {
            qDebug() << "No row selected";
            return;
        }

        // 获取选中行的第一个单元格的行号
        int selectedRow = selectedItems.first()->row();
        qDebug() << "Selected row:" << selectedRow;

        // 获取选中行所有列的内容

        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableWidget->item(selectedRow, col);
            if (item) {
                // 获取每个单元格的内容，转换为 QVariant
                rowData.append(item->text());
            }
        }
        // 打印选中行的所有数据
        qDebug() << "Selected row data:";
        for (const QVariant& data : rowData) {
            qDebug() << data.toString();
        }
    });

#endif

#endif
#if 1 // 删除数据按钮
            connect(ui->delBtn, QPushButton::clicked, this, [=]{
                qDebug() << "删除数据" << endl;
                // 首先区分查询的时候是那个被触发
                if(flagsBtn == 0){
                    qDebug() << "操作者" << endl;
                    // 首先判断用户是否选中了某行如果没有则不删除
                    // 数据库中的数据如果和QList<QVariant> rowData; 中的数据一样则删除此条消息
                    // 检查是否有选中的行
                    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
                    if (selectedItems.isEmpty()) {
                        qDebug() << "No row selected. Deletion aborted.";
                        return;
                    }

                    // 获取选中行的索引
                    int selectedRow = selectedItems.first()->row();
                    qDebug() << "Selected row for deletion:" << selectedRow;

                    // 获取选中行的数据
                    rowData.clear();  // 清空 rowData 以便存储新选中行的数据
                    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
                        QTableWidgetItem* item = ui->tableWidget->item(selectedRow, col);
                        if (item) {
                            rowData.append(item->text());  // 将选中行的每列数据添加到 rowData
                        }
                    }

                    // 打印选中行的数据
                    qDebug() << "Selected row data for deletion:";
                    for (const QVariant& data : rowData) {
                        qDebug() << data.toString();
                    }

                    // 删除数据库中的相同数据
                    bool success = deleteDataFromDatabase(database, "operatorTb", rowData);
                    if (success) {
                        qDebug() << "删除行成功!!!";
                    } else {
                        qDebug() << "删除失败";
                    }

                    // 刷新表格，移除选中行
                    ui->tableWidget->removeRow(selectedRow);
                    return;

                }else if(flagsBtn == 1){
                    // 首先判断用户是否选中了某行如果没有则不删除
                    // 数据库中的数据如果和QList<QVariant> rowData; 中的数据一样则删除此条消息
                    // 检查是否有选中的行
                    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
                    if (selectedItems.isEmpty()) {
                        qDebug() << "No row selected. Deletion aborted.";
                        return;
                    }

                    // 获取选中行的索引
                    int selectedRow = selectedItems.first()->row();
                    qDebug() << "Selected row for deletion:" << selectedRow;

                    // 获取选中行的数据
                    rowData.clear();  // 清空 rowData 以便存储新选中行的数据
                    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
                        QTableWidgetItem* item = ui->tableWidget->item(selectedRow, col);
                        if (item) {
                            rowData.append(item->text());  // 将选中行的每列数据添加到 rowData
                        }
                    }

                    // 打印选中行的数据
                    qDebug() << "Selected row data for deletion:";
                    for (const QVariant& data : rowData) {
                        qDebug() << data.toString();
                    }

                    // 删除数据库中的相同数据
                    bool success = deleteDataFromDatabase(database, "inspectorTb", rowData);
                    if (success) {
                        qDebug() << "删除行成功!!!";
                    } else {
                        qDebug() << "删除失败";
                    }

                    // 刷新表格，移除选中行
                    ui->tableWidget->removeRow(selectedRow);
                    return;
                }else {
                    qDebug() << "请选择查询" << endl;
                    return;
                }
            });

#endif
#if 1 // 修改数据按钮
            connect(ui->fixBtn, QPushButton::clicked, this, [=]{
                qDebug() << "修改数据" << endl;
                if(flagsBtn == 0){
                    qDebug() << "操作者" << endl;
                    int selectedRow = ui->tableWidget->currentRow();
                    if (selectedRow == -1) {
                        qDebug() << "没有选中任何行!";
                        return;  // If no row is selected, do nothing
                    }
                    rowData.append(ui->tableWidget->item(selectedRow, 0)->text());
                    for (int i = 0; i < dataList.size(); ++i) {
                        const QList<QVariant> &row = dataList[i];
                        // 比较第二个字段和第三个字段与选中的 tableWidget 行数据
                        if (row[1].toString() == ui->tableWidget->item(selectedRow, 0)->text()) {

                            // 如果匹配，将第一个字段赋给 index
                            index3 = row[0].toInt();
                            break;  // 找到匹配的行后可以退出循环
                        }
                    }
                    if (index3 != -1) {
                        qDebug() << "Found matching row. Index: " << index3;
                    } else {
                        qDebug() << "No matching row found.";
                    }
                    w1.setRowData(selectedRow, rowData, flagsBtn);
                    connect(&w1, &Form2_1Fix::dataUpdated1, this, [=](int updatedRow, const QString &newStreetData) {
                        // Update the tableWidget with new data from Form3Fix
                        ui->tableWidget->item(updatedRow, 0)->setText(newStreetData);
                        QString _userName = newStreetData;
                        bool flags = UserCreate::updateDataInTable(database, "operatorTb", index3, _userName);
                        if(flags)
                            qDebug()  << "更新成功" << endl;
                    });
                }else if(flagsBtn == 1){
                    qDebug() << "检查者" << endl;
                    int selectedRow = ui->tableWidget->currentRow();
                    if (selectedRow == -1) {
                        qDebug() << "没有选中任何行!";
                        return;  // If no row is selected, do nothing
                    }
                    rowData.append(ui->tableWidget->item(selectedRow, 0)->text());
                    for (int i = 0; i < dataList.size(); ++i) {
                        const QList<QVariant> &row = dataList[i];
                        // 比较第二个字段和第三个字段与选中的 tableWidget 行数据
                        if (row[1].toString() == ui->tableWidget->item(selectedRow, 0)->text()) {

                            // 如果匹配，将第一个字段赋给 index
                            index3 = row[0].toInt();
                            break;  // 找到匹配的行后可以退出循环
                        }
                    }
                    if (index3 != -1) {
                        qDebug() << "Found matching row. Index: " << index3;
                    } else {
                        qDebug() << "No matching row found.";
                    }
                    w1.setRowData(selectedRow, rowData, flagsBtn);
                    connect(&w1, &Form2_1Fix::dataUpdated2, this, [=](int updatedRow, const QString &newStreetData) {
                        // Update the tableWidget with new data from Form3Fix
                        ui->tableWidget->item(updatedRow, 0)->setText(newStreetData);
                        QString _userName = newStreetData;
                        bool flags = UserCreate::updateDataInTable(database, "inspectorTb", index3, _userName);
                        if(flags)
                            qDebug()  << "更新成功" << endl;
                    });

                }else{
                    qDebug() << "请先查询" << endl;
                }
                w1.show();
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
    flagsBtn = 0;
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
    flagsBtn = 1;
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


/**
 * @brief 判断是否为指定的数据库文件，当点击表格中的行时点击删除则会从数据库中删除对应的行
 * @param db 数据库对象
 * @param dbName 数据库文件名（如 "D1.db"）
 * @param tableName 表名
 * @return true 查询存入容器成功
 * @return false 查询存入容器失败
 */
bool UserCreate::deleteDataFromDatabase(QSqlDatabase &db, const QString &tableName, const QList<QVariant> &rowData) {
    // 检查数据库是否打开
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return false;
    }

    QSqlQuery query(db);

    // 创建 SQL 删除语句（假设 rowData 中的内容与表中的列顺序一致）
    QString deleteSQL = QString(
        "DELETE FROM %1 WHERE u_name = :xuanName;"
    ).arg(tableName);

    query.prepare(deleteSQL);
    query.bindValue(":xuanName", rowData.at(0).toString());

    if (!query.exec()) {
        qDebug() << "Error deleting data:" << query.lastError();
        return false;
    }

    return true;
}

/**
 * @brief 更新表
 * @param db 数据库对象
 * @param dbName 数据库文件名（如 "D1.db"）
 * @param tableName 表名
 * @return true 查询存入容器成功
 * @return false 查询存入容器失败
 */

bool UserCreate::updateDataInTable(QSqlDatabase &db, const QString &tableName, int rowId, const QString &newUserName) {
    // 检查数据库是否打开
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return false;
    }

    QSqlQuery query(db);

    // 构建更新 SQL 语句，假设我们只更新 u_name 和 p_word 字段
    QString updateSQL = QString("UPDATE %1 SET u_name = :newUserName WHERE id = :id;")
        .arg(tableName);

    // 输出 SQL 查询文本，确认其正确性
    qDebug() << "Executing SQL: " << updateSQL;

    query.prepare(updateSQL);

    // 绑定新数据到查询
    query.bindValue(":newUserName", newUserName);  // 假设 newUserName 是要更新的 u_name
    query.bindValue(":id", rowId);  // 根据主键 id 更新数据

    // 调试：输出绑定的参数
    qDebug() << "Binding values:";
    qDebug() << "New User Name: " << newUserName;
    qDebug() << "ID: " << rowId;

    // 执行更新操作
    if (!query.exec()) {
        qDebug() << "Error executing update:" << query.lastError();
        return false;
    }

    qDebug() << "Data updated successfully in table" << tableName;
    return true;
}
