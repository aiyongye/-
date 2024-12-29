#include "mainwindow.h"
#include <QApplication>

#include "hstorylist.h"
#include "login.h"

int main(int argc, char *argv[])
{
    try{
    QApplication a(argc, argv);
    // 启用内存泄漏检查
//    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context, const QString &msg) {
//        if (type == QtWarningMsg) {
//            qDebug() << "Memory leak detected:" << msg;
//        }
//    });
//    MainWindow w;
//    w.show();
    // 连接信号槽，确保最后一个窗口关闭时退出应用程序
    QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    Login w2;
    w2.show();


//    // 启用内存泄漏检查
//    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] %{type} %{file}:%{line} "
//                       "%{function} - %{message}");
//    // 启用内存泄漏追踪
//    QLoggingCategory::setFilterRules("qt.*.debug=true");

    return a.exec();
    }catch(const std::exception &e){
        qDebug() << "Unhandled exception:" << e.what();
    }
}
