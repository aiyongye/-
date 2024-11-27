#ifndef PRESSSTD_H
#define PRESSSTD_H

#include <QMainWindow>

namespace Ui {
class PressStd;
}

class PressStd : public QMainWindow
{
    Q_OBJECT

public:
    explicit PressStd(QWidget *parent = 0);
    ~PressStd();

private:
    Ui::PressStd *ui;
};

#endif // PRESSSTD_H
