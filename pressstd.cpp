#include "pressstd.h"
#include "ui_pressstd.h"

PressStd::PressStd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PressStd)
{
    ui->setupUi(this);

}

PressStd::~PressStd()
{
    delete ui;
}
