#include "historyform1.h"
#include "ui_historyform1.h"

historyForm1::historyForm1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::historyForm1)
{
    ui->setupUi(this);
    setFixedSize(300, 220);
    setWindowIcon(QIcon(":/img/QiTaImg.svg"));
    setWindowTitle("修改");
}
/*--------------------20241227-------------------------*/
void historyForm1::setRowData(int row, const QList<QVariant> &data)
{
    rowIndex = row;  // Store the row index
    rowData = data;  // Store the row data

//    // Access QLineEdits and set their values
    QLineEdit *lineEdit1 = findChild<QLineEdit*>("userName");
    QLineEdit *lineEdit2 = findChild<QLineEdit*>("userPass");

    if (lineEdit1 && lineEdit2) {
        lineEdit1->setText(data[0].toString());  // Assuming the first column is streetData
        lineEdit2->setText(data[1].toString());  // Assuming the second column is pressDate
    } else {
        qDebug() << "Error: Missing QLineEdit widgets!";
    }

}
/*--------------------20241227-------------------------*/

historyForm1::~historyForm1()
{
    delete ui;
}

/*--------------------20241227-------------------------*/
void historyForm1::on_liJiFixBtn_clicked()
{
    QLineEdit *lineEdit1 = findChild<QLineEdit*>("userName");
    QLineEdit *lineEdit2 = findChild<QLineEdit*>("userPass");

    if (lineEdit1 && lineEdit2) {
        QString newStreetData = lineEdit1->text();
        QString newPressDate = lineEdit2->text();

        emit dataUpdated12(rowIndex, newStreetData, newPressDate);

    } else {
        qDebug() << "Error: Missing QLineEdit widgets!";
    }
}

/*--------------------20241227-------------------------*/

