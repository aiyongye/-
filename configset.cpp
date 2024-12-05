#include "configset.h"
#include "ui_configset.h"

ConfigSet::ConfigSet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConfigSet)
{
    setAttribute(Qt::WA_QuitOnClose, false);  // 其他窗口关闭时不退出应用程序
    ui->setupUi(this);
    setFixedSize(1200,800);
//    qInstallMessageHandler(qDebugLogInfo::customMessageHandler); //打印日志
    QFile file(":/Tabs.qss");
    file.open(QFile::ReadOnly);
    QString qss=file.readAll();
    file.close();

    applyStyles(this,qss);

    loadTabWidget1(ui->tabWidget->currentIndex());


}

ConfigSet::~ConfigSet()
{
    delete ui;
}

void ConfigSet::applyStyles(QWidget *widget,QString stylesheet)
{
    widget->setStyleSheet(stylesheet); // 使用上面读取到的stylesheet
    // 遍历所有子控件并递归应用样式
    for (auto child : widget->findChildren<QWidget*>()) {
        applyStyles(child,stylesheet);
    }
}

// 重写paintEvent事件处理函数，用于绘制线段
void ConfigSet::paintEvent(QPaintEvent *)
{
    QPainter painter2(this);
    QColor color2(QString("#e4e7ed"));
    QPen pen2(color2, 2); // 这里的1是线条的宽度
    painter2.setPen(pen2);
    painter2.drawLine(xStart2, y2+2, xEnd2, y2+2); // 长线

    QPainter painter(this);
    QColor color(QString("#409eff"));///409eff
    QPen pen(color, 2); // 这里的1是线条的宽度
    painter.setPen(pen);
    painter.drawLine(xStart, y+2, xEnd, y+2);// 短线


    QPainter painter5(this);
    QColor color5(QString("#e4e7ed"));///e4e7ed
    QPen pen5(color5, 2); // 这里的1是线条的宽度
    painter5.setPen(pen5);
    painter5.drawLine(xStart5, y5+2, xEnd5, y5+2);// 长线

    QPainter painter3(this);
    QColor color3(QString("#ffffff"));
    QPen pen3(color3, 2); // 这里的1是线条的宽度
    painter3.setPen(pen3);
    painter3.drawLine(xStart3, y3+2, xEnd3, y3+2);// 短线

}


void ConfigSet::on_tabWidget_tabBarClicked(int index)
{
    loadTabWidget1(index);
    // 更新窗口，触发paintEvent重新绘制
    update();
}


void ConfigSet::loadTabWidget1(int index){
    // 当前Tab改变时，获取Tab按钮的坐标
    QRect tabRect = ui->tabWidget->tabBar()->tabRect(index);

    // 获取QTabWidget的tab栏部件
    QTabBar *tabBar = ui->tabWidget->tabBar();
    QPoint positionInTabWidget = tabRect.bottomLeft(); // 相对于QTabWidget的位置
    // 如果需要相对于主窗口的位置
    QPoint positionInMainWindow = tabBar->mapTo(ui->tabWidget->window(), positionInTabWidget);

    y=tabBar->mapTo(ui->tabWidget->window(), tabRect.bottomLeft()).y();
    xStart=tabBar->mapTo(ui->tabWidget->window(), tabRect.bottomLeft()).x();
    xEnd =tabBar->mapTo(ui->tabWidget->window(), tabRect.bottomRight()).x();

     QPoint localPos = ui->tabWidget->pos();

     xStart2=localPos.x();
     xEnd2=localPos.x()+ui->tabWidget->width();
     y2=localPos.y()+40;
}

void ConfigSet::loadTabWidget2(int index){


    // 获取QTabWidget的tab栏部件

    // 如果需要相对于主窗口的位置

}

void ConfigSet::on_tabWidget_2_tabBarClicked(int index)
{
    loadTabWidget2(index);
    // 更新窗口，触发paintEvent重新绘制
    update();
}
