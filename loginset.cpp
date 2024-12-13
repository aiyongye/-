#include "loginset.h"
#include "ui_loginset.h"


int  index = -1;

LoginSet::LoginSet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginSet)
{
    ui->setupUi(this);
//     qInstallMessageHandler(qDebugLogInfo::customMessageHandler); //打印日志
    database = LoginSet::getDatabaseConnection("./D1.db");//数据库连接
    //去除选中虚线框
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//只读 不允许编辑 (整表)
    // 设置选中行的行为
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 还可以设置选择模式为单选
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->findUserName->setDisabled(true);
    ui->findUserName->setStyleSheet(
        "QLineEdit {"
        "  user-select: none;"            // 禁止文本选择
        "  cursor: not-allowed;"          // 禁用光标
        "  background-color: #f0f0f0;"    // 背景色（可选，设置为灰色）
        "  border: 1px solid #d3d3d3;"    // 边框（可选）
        "  color: #888888;"               // 字体颜色（可选）
        "}");

    // 处理创建用户
    connect(ui->creUserBtn, QPushButton::clicked, this, [=]{
        QString userName = ui->userName->text();
        QString userpass = ui->userPass->text();
        // 获取当前日期和时间
        QDateTime currentDateTime = QDateTime::currentDateTime();

        // 格式化为 "yyyy-MM-dd HH:mm:ss" 形式（包括秒）
        QString creDataTime = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");

        qDebug() << "Formatted date and time: " <<userName <<"->"<< userpass<<"->" << creDataTime << endl;
        bool flags = LoginSet::ensureTableInDatabase(database, "./D1.db", "userPass");
        if(flags)
            qDebug() << "表创建成功" << endl;
        flags = LoginSet::insertDataIntoTable(database, "userPass", userName,userpass, creDataTime);
    });
    // 查找用户数据在ui->tableWidget上显示
    connect(ui->findBtn, QPushButton::clicked, this,[=]{
        qDebug() << "查询开始" << endl;
        // 假设 dataList 是你的查询结果
        bool flags = LoginSet::queryAllDataFromTable(database, "userPass", dataList);
        if (flags)
            qDebug() << "查询成功" << endl;

        // 清空之前的数据
        ui->tableWidget->clearContents();  // 清空表格内容
        ui->tableWidget->setRowCount(0);   // 删除所有行

        // 设置表格的列数，减去 id 列（假设你不显示 id）
        ui->tableWidget->setColumnCount(dataList.isEmpty() ? 0 : dataList.first().size() - 1);
        // 设置表格的行数
        ui->tableWidget->setRowCount(dataList.size());  // 设置表格行数
        ui->tableWidget->setColumnWidth(0, 200);  // 第 1 列宽度为 150
        ui->tableWidget->setColumnWidth(1, 200);  // 第 2 列宽度为 200
        ui->tableWidget->setColumnWidth(2, 650);  // 第 2 列宽度为 200
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
    });
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
#if 1 //当我鼠标选中ui->tableWidget中某一行时
      // 获取当前行所有内容
      // 再通过点击删除按钮，将这一行在ui->tableWidget中移除，并且在数据库中移除
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
#if 1 //当我鼠标选中ui->tableWidget中某一行时
      // 获取当前行所有内容
      // 再通过点击删除按钮，将这一行在ui->tableWidget中移除，并且在数据库中移除
    connect(ui->delBtn, QPushButton::clicked, this,[=]{
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
        bool success = deleteDataFromDatabase(database, "userPass", rowData);
        if (success) {
            qDebug() << "删除行成功!!!";
        } else {
            qDebug() << "删除失败";
        }

        // 刷新表格，移除选中行
        ui->tableWidget->removeRow(selectedRow);
    });

#endif

#if 1 // 用户修改
    connect(ui->fixBtn, &QPushButton::clicked, this, [=]() {
        qDebug() << "数据修改";

        // Get the selected row in the tableWidget
        int selectedRow = ui->tableWidget->currentRow();  // Get the index of the selected row
        if (selectedRow == -1) {
            qDebug() << "没有选中任何行!";
            return;  // If no row is selected, do nothing
        }
        // Get the data of the selected row
        QList<QVariant> rowData;  // Create a QList<QVariant> to store the selected row data
        rowData.append(ui->tableWidget->item(selectedRow, 0)->text());  // streetData
        rowData.append(ui->tableWidget->item(selectedRow, 1)->text());  // pressDate
        // 便利 容器QList<QList<QVariant>> dataList 如果第二个字段和第三个字段
        // 和ui->tableWidget->item(selectedRow, 0)->text() ui->tableWidget->item(selectedRow, 1)->text()
        // 相等 把第一个字段 赋给  int index

        // 假设 dataList 是 QList<QList<QVariant>> 类型，存储了所有数据
        for (int i = 0; i < dataList.size(); ++i) {
            const QList<QVariant> &row = dataList[i];
            // 比较第二个字段和第三个字段与选中的 tableWidget 行数据
            if (row[1].toString() == ui->tableWidget->item(selectedRow, 0)->text() &&
                row[2].toString() == ui->tableWidget->item(selectedRow, 1)->text()) {

                // 如果匹配，将第一个字段赋给 index
                index = row[0].toInt();
                break;  // 找到匹配的行后可以退出循环
            }
        }

        if (index != -1) {
            qDebug() << "Found matching row. Index: " << index;
        } else {
            qDebug() << "No matching row found.";
        }

        // Create the Form3Fix object and pass the selected row and data
//        Form3Fix w1;
        w1.setRowData(selectedRow, rowData);  // Pass selected row index and data to Form3Fix

        // Connect the signal to update data back to loginset
        connect(&w1, &Form3Fix::dataUpdated, this, [=](int updatedRow, const QString &newStreetData, const QString &newPressDate) {
            // Update the tableWidget with new data from Form3Fix
            ui->tableWidget->item(updatedRow, 0)->setText(newStreetData);
            ui->tableWidget->item(updatedRow, 1)->setText(newPressDate);
            QString _userName = newStreetData;
            QString _userPass = newPressDate;
            bool flags = LoginSet::updateDataInTable(database, "userPass", index, _userName, _userPass);
            if(flags)
                qDebug()  << "更新成功" << endl;
        });
        // Show Form3Fix to modify the selected row data
        w1.setWindowModality(Qt::ApplicationModal);
        w1.show();
//        bool flags = LoginSet::updateDataInTable(database, "userPass" , index, const QList<QVariant> &newData);

    });

#endif
#endif

}

LoginSet::~LoginSet()
{
    delete ui;
}

/**
 * @brief 获取指定名称的数据库连接
 * @param dbName 数据库文件名
 * @return QSqlDatabase 返回数据库连接对象
 */
QSqlDatabase LoginSet::getDatabaseConnection(const QString &dbName) {
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
bool LoginSet::ensureTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName) {
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
        "    p_word TEXT NOT NULL, "
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
bool LoginSet::insertDataIntoTable(QSqlDatabase &db, const QString &tableName, const QString &userName, const QString &userPass, const QString &upDate) {
    // 检查数据库是否打开
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return false;
    }

    QSqlQuery query(db);

    // 插入数据的 SQL 语句
    QString insertSQL = QString(
        "INSERT INTO %1 (u_name, p_word, up_date) "
        "VALUES (:userName, :userPass, :upDate);"
    ).arg(tableName);

    query.prepare(insertSQL);
    query.bindValue(":userName", userName);
    query.bindValue(":userPass", userPass);
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
bool LoginSet::queryAllDataFromTable(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList) {
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
 * @brief 判断是否为指定的数据库文件，当点击表格中的行时点击删除则会从数据库中删除对应的行
 * @param db 数据库对象
 * @param dbName 数据库文件名（如 "D1.db"）
 * @param tableName 表名
 * @return true 查询存入容器成功
 * @return false 查询存入容器失败
 */
bool LoginSet::deleteDataFromDatabase(QSqlDatabase &db, const QString &tableName, const QList<QVariant> &rowData) {
    // 检查数据库是否打开
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return false;
    }

    QSqlQuery query(db);

    // 创建 SQL 删除语句（假设 rowData 中的内容与表中的列顺序一致）
    QString deleteSQL = QString(
        "DELETE FROM %1 WHERE u_name = :xuanName AND p_word = :pressStd AND up_date = :pressDate;"
    ).arg(tableName);

    query.prepare(deleteSQL);
    query.bindValue(":xuanName", rowData.at(0).toString());
    query.bindValue(":pressStd", rowData.at(1).toString());
    query.bindValue(":pressDate", rowData.at(2).toString());

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

bool LoginSet::updateDataInTable(QSqlDatabase &db, const QString &tableName, int rowId, const QString &newUserName, const QString &newPassword) {
    // 检查数据库是否打开
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return false;
    }

    QSqlQuery query(db);

    // 构建更新 SQL 语句，假设我们只更新 u_name 和 p_word 字段
    QString updateSQL = QString("UPDATE %1 SET u_name = :newUserName, p_word = :newPassword WHERE id = :id;")
        .arg(tableName);

    // 输出 SQL 查询文本，确认其正确性
    qDebug() << "Executing SQL: " << updateSQL;

    query.prepare(updateSQL);

    // 绑定新数据到查询
    query.bindValue(":newUserName", newUserName);  // 假设 newUserName 是要更新的 u_name
    query.bindValue(":newPassword", newPassword);  // 假设 newPassword 是要更新的 p_word
    query.bindValue(":id", rowId);  // 根据主键 id 更新数据

    // 调试：输出绑定的参数
    qDebug() << "Binding values:";
    qDebug() << "New User Name: " << newUserName;
    qDebug() << "New Password: " << newPassword;
    qDebug() << "ID: " << rowId;

    // 执行更新操作
    if (!query.exec()) {
        qDebug() << "Error executing update:" << query.lastError();
        return false;
    }

    qDebug() << "Data updated successfully in table" << tableName;
    return true;
}
