#ifndef FORM3FIX_H
#define FORM3FIX_H

#include <QMainWindow>

namespace Ui {
class Form3Fix;
}

class Form3Fix : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form3Fix(QWidget *parent = 0);
    ~Form3Fix();

private:
    Ui::Form3Fix *ui;


public:
    void setRowData(int row, const QList<QVariant> &data);
    void on_updateBtn_clicked();
    QList<QVariant> rowData;  // Member to store the passed data
    int rowIndex;  // Store the index of the selected row

signals:
    void dataUpdated(int row, const QString &newStreetData, const QString &newPressDate);

};

#endif // FORM3FIX_H
