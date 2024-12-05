#include "form1fix.h"
#include "ui_form1fix.h"

Form1Fix::Form1Fix(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form1Fix)
{
    ui->setupUi(this);
    setFixedSize(300, 220);
    qInstallMessageHandler(qDebugLogInfo::customMessageHandler); //打印日志
    connect(ui->liJiXiuGaiBtn, QPushButton::clicked, this, [=]{
        qDebug() << "立即修改" << endl;
        Form1Fix::on_updateBtn_clicked();

    });
}

Form1Fix::~Form1Fix()
{
    delete ui;
}
void Form1Fix::setRowData(int row, const QList<QVariant> &data)
{
    rowIndex = row;  // Store the row index
    rowData = data;  // Store the row data

    // Access QLineEdits and set their values
    QLineEdit *lineEdit1 = findChild<QLineEdit*>("xuanGuaName");
    QLineEdit *lineEdit3 = findChild<QLineEdit*>("startStd");
    QLineEdit *lineEdit4 = findChild<QLineEdit*>("endStd");

    if (lineEdit1 && lineEdit3 && lineEdit4) {
        lineEdit1->setText(data[0].toString());  // Assuming the first column is streetData

        // Split the data[1] (5~10) into two parts based on "~"
        QString buf = data[1].toString();
        QStringList parts = buf.split("~");

        if (parts.size() == 2) {
            // Assuming you want to set start and end values separately
            QString startStd = parts[0];  // "5"
            QString endStd = parts[1];    // "10"

            lineEdit3->setText(startStd);  // Set start date value
            lineEdit4->setText(endStd);    // Set end date value
        } else {
            qDebug() << "Error: Invalid format in data[1], expected 'start~end' format.";
        }
    } else {
        qDebug() << "Error: Missing QLineEdit widgets!";
    }
}



void Form1Fix::on_updateBtn_clicked()
{
    QLineEdit *lineEdit1 = findChild<QLineEdit*>("xuanGuaName");
    QLineEdit *lineEdit2Start = findChild<QLineEdit*>("startStd");
    QLineEdit *lineEdit2End = findChild<QLineEdit*>("endStd");

    if (lineEdit1 && lineEdit2Start && lineEdit2End) {
        QString newStreetData = lineEdit1->text();
        QString newPressDate = lineEdit2Start->text() + "~" + lineEdit2End->text();

        // Emit the signal with updated data and row index
        emit dataUpdated(rowIndex, newStreetData, newPressDate);
    } else {
        qDebug() << "Error: Missing QLineEdit widgets!";
    }
}
