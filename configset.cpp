#include "configset.h"
#include "ui_configset.h"

ConfigSet::ConfigSet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConfigSet)
{
    ui->setupUi(this);

    // 设置窗口标题和大小
        setWindowTitle("表单");
        setFixedSize(1200, 800);

}

ConfigSet::~ConfigSet()
{
    delete ui;
}
