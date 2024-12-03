#include "form2_1fix.h"
#include "ui_form2_1fix.h"

Form2_1Fix::Form2_1Fix(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form2_1Fix)
{
    ui->setupUi(this);
    setFixedSize(300, 220);
}

Form2_1Fix::~Form2_1Fix()
{
    delete ui;
}
