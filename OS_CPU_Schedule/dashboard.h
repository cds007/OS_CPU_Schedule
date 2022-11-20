#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QtGlobal>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>

namespace Ui {
class Dashboard;
}

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();
    void paintEvent(QPaintEvent *event);//重绘事件
    void updateValue(double value);//更新展示的值，触发事件重绘
    void drawLines(QPainter *painter);//绘制最外围的线
    void drawBGE(QPainter *painter);//绘制中间最外层背景
    void drawTextE(QPainter *painter);//绘制中心文字背景
    void drawText(QPainter *painter);//绘制中心文字

    void delValue();//值减一
    void addValue();//值加一

    int getValue() const;

private:
    Ui::Dashboard *ui;
    int lineCount;  //总的最外层线的条数
    double value;      //值
    int textSize;   //文本大小
    int bgERadius;  //背景直径
    int outLineRadius; //外层线的直径
    int innLineRadius; //内层线的直径
    int textOutRadius; //文本背景外层直径
    int textInnRadius; //文本背景内存直径
};

#endif // DASHBOARD_H
