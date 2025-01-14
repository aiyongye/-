#include "modbusconn.h"

ModbusConn::ModbusConn()
{

}
void MainWindow::startRefun1() {
    if (!modbusDevice) {
        qDebug() << "modbusDevice is not initialized!";
        return;
    }

//    qDebug() << "Current state:" << modbusDevice->state();

    // 如果 Modbus 未处于连接状态，尝试连接
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, 502);
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, "192.168.0.170");
        modbusDevice->setTimeout(2000);  // 设置超时时间
        modbusDevice->setNumberOfRetries(2);  // 设置重试次数

//        qDebug() << "Attempting to connect to:"
//                 << modbusDevice->connectionParameter(QModbusDevice::NetworkAddressParameter).toString()
//                 << "Port:"
//                 << modbusDevice->connectionParameter(QModbusDevice::NetworkPortParameter).toInt();

        // 捕获连接过程中的错误
        connect(modbusDevice, &QModbusDevice::errorOccurred, this, [this](QModbusDevice::Error error) {
            qDebug() << "Error occurred:" << error << " - " << modbusDevice->errorString();
        });

        // 捕获状态变化
        connect(modbusDevice, &QModbusDevice::stateChanged, this, [](QModbusDevice::State state) {
            qDebug() << "State changed to:" << state;
        });

        if (!modbusDevice->connectDevice()) {
            qDebug() << "Connection attempt failed immediately: " << modbusDevice->errorString();
            return;
        }

        qDebug() << "Connection attempt started...";
//        return; // 等待连接完成后再执行后续操作
    } else {
//        qDebug() << "Already connected. Proceeding to read.";
    }
    // 准备读取操作
    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, 1, 1);  // 寄存器起始地址3，读取1个寄存器
//    qDebug() << "device. State:-------------" << modbusDevice->state();


    if (auto *reply = modbusDevice->sendReadRequest(readUnit, 1)) {  // 设备地址为1
        if (!reply->isFinished()) {
//            qDebug() << "reoly -------" << endl;
            connect(reply, &QModbusReply::finished, this, &MainWindow::toReadReady);
        } else {
//            qDebug() << "Broadcast reply finished immediately.";
            reply->deleteLater();  // 广播请求立即完成时，清理回复对象
        }
    } else {
        qDebug() << "Send read request failed: " << modbusDevice->errorString();
    }

    QThread::msleep(1);  // 延时1ms（一般不需要，避免使用可能导致非必要的性能问题）
}

void MainWindow::toReadReady()
{
//QModbusReply这个类存储了来自client的数据,sender()返回发送信号的对象的指针
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        return;
    }
    if (reply->error() == QModbusDevice::NoError)
    {
        //处理成功返回的数据
       const QModbusDataUnit unit = reply->result();
        //quint16 stat = unit.value(1);  //状态（位与关系）

         MainWindow::a = unit.value(0);
//       qDebug() << "hahah" <<endl;
//       qDebug()<<"plc值"<<unit.value(0);

       zhanKaiLine->setText(QString::number(MainWindow::a));

//       timer->start(1000);
//       elapsedTimer->start();
//       qDebug()

    }
}
