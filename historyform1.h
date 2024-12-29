#ifndef HISTORYFORM1_H
#define HISTORYFORM1_H

#include <QMainWindow>
#include <QDebug>
namespace Ui {
class historyForm1;
}

class historyForm1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit historyForm1(QWidget *parent = 0);
    ~historyForm1();

private:
    Ui::historyForm1 *ui;
    /*--------------------20241227-------------------------*/
public:
void setRowData(int row, const QList<QVariant> &data);
QList<QVariant> rowData;  // Member to store the passed data
int rowIndex;  // Store the index of the selected row
signals:
    void dataUpdated12(int row, const QString &newStreetData, const QString &newPressDate);
    /*--------------------20241227-------------------------*/
/*--------------------20241227-------------------------*/
private slots:
    void on_liJiFixBtn_clicked();
    /*--------------------20241227-------------------------*/
};

#endif // HISTORYFORM1_H
