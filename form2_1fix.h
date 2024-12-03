#ifndef FORM2_1FIX_H
#define FORM2_1FIX_H

#include <QMainWindow>

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
};

#endif // FORM2_1FIX_H
