#include "hstorylist.h"
#include "ui_hstorylist.h"

HstoryList::HstoryList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HstoryList)
{
    ui->setupUi(this);
}

HstoryList::~HstoryList()
{
    delete ui;
}
