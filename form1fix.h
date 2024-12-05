#ifndef FORM1FIX_H
#define FORM1FIX_H

#include <QMainWindow>
#include <QDebug>
#include "qdebugloginfo.h"
namespace Ui {
class Form1Fix;
}

class Form1Fix : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form1Fix(QWidget *parent = 0);
    ~Form1Fix();
public:
    QList<QVariant> rowData;
    int rowIndex;
public:
    void setRowData(int row, const QList<QVariant> &data);
    void on_updateBtn_clicked();
signals:
    void dataUpdated(int row, const QString &newStreetData, const QString &newPressDate);

private:
    Ui::Form1Fix *ui;
};

#endif // FORM1FIX_H
