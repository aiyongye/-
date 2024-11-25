#include "sqliteaction.h"

SqliteAction::SqliteAction()
{

}

/**
 * @brief 获取指定名称的数据库连接
 * @param dbName 数据库文件名
 * @return QSqlDatabase 返回数据库连接对象
 */
QSqlDatabase SqliteAction::getDatabaseConnection(const QString &dbName) {
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
bool SqliteAction::ensureTableInDatabase(QSqlDatabase &db, const QString &dbName, const QString &tableName) {
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
bool SqliteAction::insertIntoTable(QSqlDatabase &db, const QString &dbName, const QString &tableName, const QList<QString> &columns, const QList<QVariant> &values) {
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

