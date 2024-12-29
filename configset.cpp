#include "configset.h"
#include "ui_configset.h"

ConfigSet::ConfigSet(QWidget *parent) :
    QMainWindow(parent),
    pressStdLoaded(false),
    userCreateLoaded(false),
    loginSetLoaded(false),
    pressStd(nullptr),
    userCreate(nullptr),
    loginSet(nullptr),
    ui(new Ui::ConfigSet)
{
    setAttribute(Qt::WA_QuitOnClose, false);  // 其他窗口关闭时不退出应用程序
    ui->setupUi(this);
    setFixedSize(1200,800);
    setWindowIcon(QIcon(":/img/SetImg.svg"));
    setWindowTitle("配置");



#if 1 //处理ui刷新
    // 确保每个标签页都有布局
       QVBoxLayout *layoutTab9 = new QVBoxLayout();
       ui->tabWidget->widget(0)->setLayout(layoutTab9); // 给 tab_9 设置布局

       QVBoxLayout *layoutTab10 = new QVBoxLayout();
       ui->tabWidget->widget(1)->setLayout(layoutTab10); // 给 tab_10 设置布局

       QVBoxLayout *layoutTab11 = new QVBoxLayout();
       ui->tabWidget->widget(2)->setLayout(layoutTab11); // 给 tab_11 设置布局

       // 连接 tabWidget 的 currentChanged 信号，当标签切换时执行懒加载
       connect(ui->tabWidget, &QTabWidget::currentChanged, this, &ConfigSet::onTabChanged);


#endif
//    qInstallMessageHandler(qDebugLogInfo::customMessageHandler); //打印日志
    QFile file(":/Tabs.qss");
    file.open(QFile::ReadOnly);
    QString qss=file.readAll();
    file.close();


    applyStyles(this,qss);

    loadTabWidget1(ui->tabWidget->currentIndex());
    ui->tabWidget->setCurrentIndex(3); // 将当前标签页设置为第 3 个标签页（索引从 0 开始）

            /***********************bash-20241210*******************/
    // 当接收到来自 AWidget 的数据时，将其转发给 CWidget
//    PressStd *aWidget = findChild<PressStd *>();
    // 如果找到了 AWidget，连接信号到槽
//    if (aWidget) {
//        connect(pressStd, &PressStd::sendDataToBWidget, this, &ConfigSet::onReceiveDataFromAWidget);
//    }
        /***********************bash-20241212*******************/

        /***********************bash-20241212*******************/
    // 当接收到来自 AWidget 的数据时，将其转发给 CWidget
//    UserCreate *aWidget2 = findChild<UserCreate *>();
    // 如果找到了 AWidget，连接信号到槽
//    if (aWidget2) {
//    connect(userCreate, &UserCreate::sendDataToBWidget, this, &ConfigSet::onReceiveDataFromAWidget2);
//    }
    /***********************bash-20241212*******************/
}

ConfigSet::~ConfigSet()
{

    delete ui;
}
            /***********************bash-20241211*******************/
// ConfigSet.cpp
void ConfigSet::onReceiveDataFromAWidget(const int &data) {
    qDebug() << "Received data in ConfigSet:" << data;
    emit sendDataBToCWidget(data);
    // 更新界面或执行其他操作
}
            /***********************bash-20241211*******************/

            /***********************bash-20241212*******************/
// ConfigSet.cpp
void ConfigSet::onReceiveDataFromAWidget2(const int &data) {
    qDebug() << "Received data in ConfigSet:" << data;
emit sendDataBToCWidget2(data);
// 更新界面或执行其他操作
}
            /***********************bash-20241212*******************/

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

    y=tabBar->mapTo(ui->tabWidget->window(), tabRect.bottomLeft()).y();
    xStart=tabBar->mapTo(ui->tabWidget->window(), tabRect.bottomLeft()).x();
    xEnd =tabBar->mapTo(ui->tabWidget->window(), tabRect.bottomRight()).x();

     QPoint localPos = ui->tabWidget->pos();

     xStart2=localPos.x();
     xEnd2=localPos.x()+ui->tabWidget->width();
     y2=localPos.y()+40;
}

void ConfigSet::onTabChanged(int index)
{
    // 根据当前选中的标签页索引加载相应的控件
    switch (index) {
        case 0: // 第一个标签页：工艺标准 (PressStd)
            if (!pressStdLoaded) {
                pressStd = new PressStd(this); // 创建 PressStd 控件
                ui->tabWidget->widget(0)->layout()->addWidget(pressStd); // 将控件添加到该标签页
                pressStdLoaded = true; // 标记为已加载
                        connect(pressStd, &PressStd::sendDataToBWidget, this, &ConfigSet::onReceiveDataFromAWidget);
            }
            break;

        case 1: // 第二个标签页：角色创建 (UserCreate)
            if (!userCreateLoaded) {
                userCreate = new UserCreate(this); // 创建 UserCreate 控件
                ui->tabWidget->widget(1)->layout()->addWidget(userCreate); // 将控件添加到该标签页
                userCreateLoaded = true; // 标记为已加载
                    connect(userCreate, &UserCreate::sendDataToBWidget, this, &ConfigSet::onReceiveDataFromAWidget2);
            }
            break;

        case 2: // 第三个标签页：角色管理 (LoginSet)
            if (!loginSetLoaded) {
                loginSet = new LoginSet(this); // 创建 LoginSet 控件
                ui->tabWidget->widget(2)->layout()->addWidget(loginSet); // 将控件添加到该标签页
                loginSetLoaded = true; // 标记为已加载
            }
            break;

        default:
            break;
    }
}

