#include "pressstd.h"
#include "ui_pressstd.h"

PressStd::PressStd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PressStd)
{
    ui->setupUi(this);
    setWindowTitle("记录查询");
    resize(1200, 800);
    // 状态栏
    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    // 设置状态栏样式
    statusBar->setStyleSheet("QStatusBar { background-color: #34495e; color: white; font-size: 12px; }");

    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    menuBar->addMenu("系统(&V)");
    menuBar->addMenu("设置(&Y)");
    menuBar->addMenu("维护(&P)");
    menuBar->addMenu("工具(&T)");
    menuBar->addMenu("帮助(&Z)");

    // 设置菜单栏样式
    menuBar->setStyleSheet("QMenuBar { background-color: #2c3e50; color: white; font-size: 14px; }"
                           "QMenuBar::item { background-color: #34495e; padding: 5px; border-radius: 5px; }"
                           "QMenuBar::item:selected { background-color: #1abc9c; }");


}

PressStd::~PressStd()
{
    delete ui;
}
