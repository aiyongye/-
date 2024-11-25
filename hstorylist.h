#ifndef HSTORYLIST_H
#define HSTORYLIST_H

#include <QMainWindow>

namespace Ui {
class HstoryList;
}

class HstoryList : public QMainWindow
{
    Q_OBJECT

public:
    explicit HstoryList(QWidget *parent = 0);
    ~HstoryList();

private:
    Ui::HstoryList *ui;
};

#endif // HSTORYLIST_H
