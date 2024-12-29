#include "login.h"
#include "ui_login.h"


Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    // 只允许英文字母和数字
    QRegExp rx("[A-Za-z0-9]*");
    QRegExpValidator *validator = new QRegExpValidator(rx, this);
    ui->passLine->setValidator(validator);

    //设置窗体背景色为黑色
//    setStyleSheet("QWidget{background:black}");
//    setWindowOpacity(0.8);//设置窗口的透明度

    //-------------bash20241215--------------
    // 这里启动后台资源加载，即使用户还没登录
//     QtConcurrent::run(this, &Login::loadW1ResourcesInBackground);
     w1 = nullptr;  // 初始化为 nullptr
    //-------------bash20241215--------------
    setWindowIcon(QIcon(":/img/LoginImg.svg"));
    setWindowTitle("登录");
    database = Login::getDatabaseConnection("./D1.db");
    Login::findTableToConBox();
    Login::showLoginPage();

    connect(ui->loginBtn, QPushButton::clicked, this, [=]{
        qDebug() << "登录界面即将跳转主界面" << endl;
        QString enteredUserName = ui->userComBox->currentText();
        QString enteredPassword = ui->passLine->text();
        // 检查用户名和密码是否匹配
        bool isValidUser = false;
        for (const QList<QVariant>& rowData : dataList) {
            if (rowData.size() >= 4) {  // 确保当前行有足够的字段
                QString storedUserName = rowData[1].toString();  // 获取数据库中的用户名（假设是第二列）
                QString storedPassword = rowData[2].toString();  // 获取数据库中的密码（假设是第三列）

                // 如果用户名和密码匹配
                if (enteredUserName == storedUserName && enteredPassword == storedPassword) {
                    isValidUser = true;
                    break;  // 找到匹配项，退出循环
                }
            }
        }
        // 如果匹配成功，跳转到下一个页面
        if (isValidUser) {
//            w1.show();  // 页面跳转
//            this->close();
            QMessageBox::information(this, "登录成功", "欢迎进入系统！");
            Login::onLoginSuccess();

        } else {
            // 显示错误信息（可以自定义）
            QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
        }


    });

    connect(ui->quXiaoBtn, QPushButton::clicked, this, [=]{
        qDebug() << "退出登录" << endl;
    });
}

Login::~Login()
{
    delete ui;
    if (w1) delete w1;  // 确保销毁 w1
}
/**
 * @brief 获取指定名称的数据库连接
 * @param dbName 数据库文件名
 * @return QSqlDatabase 返回数据库连接对象
 */
QSqlDatabase Login::getDatabaseConnection(const QString &dbName) {
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
 * @brief 判断是否为指定的数据库文件，查询表中所有数据存入容器中
 * @param db 数据库对象
 * @param dbName 数据库文件名（如 "D1.db"）
 * @param tableName 表名
 * @return true 查询存入容器成功
 * @return false 查询存入容器失败
 */
bool Login::queryAllDataFromTable(QSqlDatabase &db, const QString &tableName, QList<QList<QVariant>> &dataList) {
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
 * @brief 查询表将数据存到容器中 容器中的数据啊add到ComboBox中
 */
void Login::findTableToConBox(){
    bool flags = Login::queryAllDataFromTable(database, "userPass", dataList);
    if(flags)
        qDebug() << "查询userPass成功" << endl;
    // 将QList<QList<QVariant>> dataList; 存储的第二个字段的值 ui->userComBox 中
    for (const QList<QVariant>& itemList : dataList) {
        if (itemList.size() >= 4) {
            QString uName = itemList[1].toString();
            ui->userComBox->addItem(uName);
            qDebug() << uName << endl;
        }
    }

}



//-------------bash20241215--------------
void Login::showLoginPage()
{
    // 显示Login界面
    this->show();

    // 延迟加载额外资源
    QTimer::singleShot(0, this, &Login::startResourceLoading);
}

void Login::startResourceLoading()
{
    // 3. 异步加载资源，确保不阻塞 UI 线程
    QtConcurrent::run(this, &Login::loadW1ResourcesInBackground);
}

void Login::loadW1ResourcesInBackground()
{
    // 在后台加载 w1 的资源，避免阻塞 UI 线程
    qDebug() << "后台加载 w1 窗口资源...";

    qDebug() << "w1 窗口资源加载完成！";
}

void Login::onLoginSuccess()
{
    // 仅在登录成功后才创建 w1 窗口
    if (!w1) {
        w1 = new MainWindow();  // 动态创建 w1 窗口
    }
    // 异步加载 w1 的资源（如果需要的话）
    QtConcurrent::run(this, &Login::loadW1ResourcesInBackground);

    // 显示 w1 窗口并关闭 Login 窗口
    w1->show();
    this->close();
}


//-------------bash20241215--------------
