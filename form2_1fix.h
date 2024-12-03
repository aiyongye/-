#ifndef FORM2_1FIX_H
#define FORM2_1FIX_H

#include <QMainWindow>
#include <QDebug>
namespace Ui {
class Form2_1Fix;
}

class Form2_1Fix : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form2_1Fix(QWidget *parent = 0);
    ~Form2_1Fix();

private:
    Ui::Form2_1Fix *ui;
public:
    void setRowData(int row, const QList<QVariant> &data,int flagsBtn);
    void on_updateBtn_clicked();
    QList<QVariant> rowData;  // Member to store the passed data
    int rowIndex;  // Store the index of the selected row

signals:
    void dataUpdated1(int row, const QString &newStreetData);
    void dataUpdated2(int row, const QString &newStreetData);

};

#endif // FORM2_1FIX_H
