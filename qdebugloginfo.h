#ifndef QDEBUGLOGINFO_H
#define QDEBUGLOGINFO_H
#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>

class qDebugLogInfo
{
public:
    qDebugLogInfo();
    static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // QDEBUGLOGINFO_H
