#include "form2_1fix.h"
#include "ui_form2_1fix.h"
int asd = -1;
Form2_1Fix::Form2_1Fix(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form2_1Fix)
{
    ui->setupUi(this);
    setFixedSize(300, 220);
     qInstallMessageHandler(qDebugLogInfo::customMessageHandler); //打印日志
    connect(ui->liJiFixBtn, QPushButton::clicked, this, [=]{
        qDebug() << "立即修改" << endl;
        Form2_1Fix::on_updateBtn_clicked();
    });
}

Form2_1Fix::~Form2_1Fix()
{
    delete ui;
}
void Form2_1Fix::setRowData(int row, const QList<QVariant> &data,int flagsBtn)
{
    rowIndex = row;  // Store the row index
    rowData = data;  // Store the row data
    asd = flagsBtn;
    // Access QLineEdits and set their values
    QLineEdit *lineEdit1 = findChild<QLineEdit*>("userName");

    if (lineEdit1) {
        lineEdit1->setText(data[0].toString());  // Assuming the first column is streetData
    } else {
        qDebug() << "Error: Missing QLineEdit widgets!";
    }
}

void Form2_1Fix::on_updateBtn_clicked()
{
    // Get the updated data from QLineEdit widgets
    QLineEdit *lineEdit1 = findChild<QLineEdit*>("userName");

    if (lineEdit1) {
        QString newStreetData = lineEdit1->text();
        // Emit the signal with updated data and row index
        if(asd == 0)
            emit dataUpdated1(rowIndex, newStreetData);
        else
            emit dataUpdated2(rowIndex, newStreetData);
    } else {
        qDebug() << "Error: Missing QLineEdit widgets!";
    }
}
