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
#include "pressstd.h"
#include "usercreate.h"
#include "loginset.h"

            /***********************bash-20241210*******************/
#include "pressstd.h"
            /***********************bash-20241210*******************/

            /***********************bash-20241212*******************/
#include "usercreate.h"
            /***********************bash-20241212*******************/

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

     /***********************bash-20241210*******************/
public slots:
void onReceiveDataFromAWidget2(const int &data);
signals:
void sendDataBToCWidget2(const int &data);
     /***********************bash-20241210*******************/

public:
// 控件加载状态标记
bool pressStdLoaded;
bool userCreateLoaded;
bool loginSetLoaded;

// 控件指针
PressStd *pressStd;
UserCreate *userCreate;
LoginSet *loginSet;
void onTabChanged(int index);
};

#endif // CONFIGSET_H
