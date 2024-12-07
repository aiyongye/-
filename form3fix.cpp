#include "form3fix.h"
#include "ui_form3fix.h"
#include <QDebug>
#include <QPushButton>
//int index;
Form3Fix::Form3Fix(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form3Fix)
{
    ui->setupUi(this);
    setFixedSize(300, 220);
    setWindowIcon(QIcon(":/img/QiTaImg.svg"));
    setWindowTitle("修改");
//     qInstallMessageHandler(qDebugLogInfo::customMessageHandler); //打印日志
    connect(ui->liJiFixBtn, QPushButton::clicked, this, [=]{
        Form3Fix::on_updateBtn_clicked();
    });
}
void Form3Fix::setRowData(int row, const QList<QVariant> &data)
{
    rowIndex = row;  // Store the row index
    rowData = data;  // Store the row data

    // Access QLineEdits and set their values
    QLineEdit *lineEdit1 = findChild<QLineEdit*>("userName");
    QLineEdit *lineEdit2 = findChild<QLineEdit*>("userPass");

    if (lineEdit1 && lineEdit2) {
        lineEdit1->setText(data[0].toString());  // Assuming the first column is streetData
        lineEdit2->setText(data[1].toString());  // Assuming the second column is pressDate
    } else {
        qDebug() << "Error: Missing QLineEdit widgets!";
    }
}

void Form3Fix::on_updateBtn_clicked()
{
    // Get the updated data from QLineEdit widgets
    QLineEdit *lineEdit1 = findChild<QLineEdit*>("userName");
    QLineEdit *lineEdit2 = findChild<QLineEdit*>("userPass");

    if (lineEdit1 && lineEdit2) {
        QString newStreetData = lineEdit1->text();
        QString newPressDate = lineEdit2->text();

        // Emit the signal with updated data and row index
        emit dataUpdated(rowIndex, newStreetData, newPressDate);
    } else {
        qDebug() << "Error: Missing QLineEdit widgets!";
    }
}

Form3Fix::~Form3Fix()
{
    delete ui;
}
