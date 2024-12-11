#ifndef CONFIGSET_H
#define CONFIGSET_H

#include <QMainWindow>
#include <QFormLayout>
#include <QFile>
#include <QDebug>
#include <QPainter>
#include "QGraphicsScene"
#include "QGraphicsView"
#include "QGraphicsLineItem"
#include "QPaintEvent"
#include "qdebugloginfo.h"
            /***********************bash-20241210*******************/
#include "pressstd.h"
            /***********************bash-20241210*******************/
namespace Ui {
class ConfigSet;
}

class ConfigSet : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConfigSet(QWidget *parent = 0);
    ~ConfigSet();
private slots:
    void on_tabWidget_tabBarClicked(int index);
    void loadTabWidget1(int index);
    void loadTabWidget2(int index);

    void on_tabWidget_2_tabBarClicked(int index);
private:
    Ui::ConfigSet *ui;
    void applyStyles(QWidget *widget,QString stylesheet);
    int y=0;
    int xStart=0;
    int xEnd =0;
    int y2=0;
    int xStart2=0;
    int xEnd2 =0;
    int y3=0;
    int xStart3=0;
    int xEnd3 =0;
    int y5=0;
    int xStart5=0;
    int xEnd5 =0;

protected:
    void paintEvent(QPaintEvent *event) override;
            /***********************bash-20241210*******************/
public slots:
    void onReceiveDataFromAWidget(const int &data);
signals:
     void sendDataBToCWidget(const int &data);
            /***********************bash-20241210*******************/

};

#endif // CONFIGSET_H
