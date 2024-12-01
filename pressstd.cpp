#include "pressstd.h"
#include "ui_pressstd.h"


PressStd::PressStd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PressStd)
{
    ui->setupUi(this);

    QFile file(":/elementForm.qss");
    file.open(QFile::ReadOnly);
    QString qss=file.readAll();
    file.close();
        applyStyles(this,qss);
       database = PressStd::getDatabaseConnection("../qtModBus/D1.db");//数据库连接


    //引用fontawesome，加载字体图标资源
    int i_font_id = QFontDatabase::addApplicationFont(":/fontawesome-webfont.ttf");
    QStringList font_families = QFontDatabase::applicationFontFamilies(i_font_id);
    if (!font_families.empty()){
        //fontAwesome已经在qss设置了，这里就直接设置图标就可以
        //QFont fontAwesome(font_families[0]);
    }
    // 设置日历弹出模式为真，这样当点击下拉按钮时就会显示一个日历控件
    ui->dateEdit->setCalendarPopup(true);
    // 获取当前日期并设置到 QDateEdit
    QDate currentDate = QDate::currentDate();  // 获取当前日期
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit->setDate(currentDate);        // 设置当前日期到 dateEdit
    ui->timeEdit->setTime(QTime::currentTime());
#if 1 //数据创建处理
    connect(ui->liJiCreateBtn, QPushButton::clicked,this,[=]{
       // 插入数据库
       // 悬挂件名称在第一字段、压力标准开始和结束拼接在一起用~连接
       QString guaName = ui->xuanGuaName->text();
       QString crePressSE = ui->startStd->text() + "~" + ui->endStd->text();
       // 创建时间 Data 和 Time 也拼接在一起用 " " 连接
       QString credataTime =ui->dateEdit->text() + " " + ui->timeEdit->text();
       bool flags = PressStd::ensureTableInDatabase(database, "../qtModBus/D1.db", "proStds");
       if(flags)
           qDebug() << "proStds表创建成功" << endl;
       flags = PressStd::insertDataIntoTable(database, "proStds", guaName, crePressSE, credataTime);
       if(flags)
            qDebug() << "创建成功"<< guaName << "->" << crePressSE << "->" << credataTime << endl;
    });
#endif

#if 1 // 数据查询处理 目前处理全查询 按时间升序插入表格中
    // 每次点击按钮时清空之前的数据并重新插入
    connect(ui->findBtn, QPushButton::clicked, this, [=]() {
        qDebug() << "查询开始" << endl;
        // 假设 dataList 是你的查询结果
        bool flags = PressStd::queryAllDataFromTable(database, "proStds", dataList);
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
#endif
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
        bool success = deleteDataFromDatabase(database, "proStds", rowData);
        if (success) {
            qDebug() << "删除行成功!!!";
        } else {
            qDebug() << "删除失败";
        }

        // 刷新表格，移除选中行
        ui->tableWidget->removeRow(selectedRow);
    });

#endif

#if 1 //当点击修改按钮时将当前数据修改
    connect(ui->fixBtn, QPushButton::clicked, this, [=]{
        qDebug() << "修改按钮" << endl;

    });

#endif
}

PressStd::~PressStd()
{
    delete ui;
}
void PressStd::applyStyles(QWidget *widget,QString stylesheet)
{
    widget->setStyleSheet(stylesheet); // 使用上面读取到的stylesheet
    // 遍历所有子控件并递归应用样式
    for (auto child : widget->findChildren<QWidget*>()) {
        applyStyles(child,stylesheet);
    }
}
/**
 * @brief 获取指定名称的数据库连接
 * @param dbName 数据库文件名
 * @return QSqlDatabase 返回数据库连接对象
 */
QSqlDatabase PressStd::getDatabaseConnection(const QString &dbName) {
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
bool PressStd::ensureTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName) {
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
        "    xuanName TEXT NOT NULL, "
        "    press_std TEXT NOT NULL, "
        "    press_date DATE NOT NULL"
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
bool PressStd::insertDataIntoTable(QSqlDatabase &db, const QString &tableName, const QString &xuanName, const QString &pressStd, const QString &pressDate) {
    // 检查数据库是否打开
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return false;
    }

    QSqlQuery query(db);

    // 插入数据的 SQL 语句
    QString insertSQL = QString(
        "INSERT INTO %1 (xuanName, press_std, press_date) "
        "VALUES (:xuanName, :pressStd, :pressDate);"
    ).arg(tableName);

    query.prepare(insertSQL);
    query.bindValue(":xuanName", xuanName);
    query.bindValue(":pressStd", pressStd);
    query.bindValue(":pressDate", pressDate);

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
bool PressStd::queryAllDataFromTable(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList) {
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
bool PressStd::deleteDataFromDatabase(QSqlDatabase &db, const QString &tableName, const QList<QVariant> &rowData) {
    // 检查数据库是否打开
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return false;
    }

    QSqlQuery query(db);

    // 创建 SQL 删除语句（假设 rowData 中的内容与表中的列顺序一致）
    QString deleteSQL = QString(
        "DELETE FROM %1 WHERE xuanName = :xuanName AND press_std = :pressStd AND press_date = :pressDate;"
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
