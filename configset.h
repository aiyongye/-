#ifndef CONFIGSET_H
#define CONFIGSET_H
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QLineEdit>

namespace Ui {
class ConfigSet;
}

class ConfigSet : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConfigSet(QWidget *parent = 0);
    ~ConfigSet();

private:
    Ui::ConfigSet *ui;
//    QLineEdit *nameLineEdit;
//    QLineEdit *operatorLineEdit;
//    QLineEdit *inspectorLineEdit;
//    QLineEdit *pressureLineEdit;
//    QTableWidget *tableWidget;
};

#endif // CONFIGSET_H
