#include "qdebugloginfo.h"

qDebugLogInfo::qDebugLogInfo()
{

}
void qDebugLogInfo::customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch (type) {
    case QtDebugMsg:
    text = QString("Debug: %1").arg(msg);
    break;
    case QtWarningMsg:
    text = QString("Warning: %1").arg(msg);
    break;
    case QtCriticalMsg:
    text = QString("Critical: %1").arg(msg);
    break;
    case QtFatalMsg:
    text = QString("Fatal: %1").arg(msg);
    abort();
    }

    QFile outFile("./log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << text << endl;
    outFile.close();

    mutex.unlock();
}
