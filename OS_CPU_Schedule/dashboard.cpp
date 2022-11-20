#include "dashboard.h"
#include "ui_dashboard.h"

Dashboard::Dashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dashboard)
{
    ui->setupUi(this);
    lineCount = 100;
    value = 100;
    bgERadius = 30;
    textSize = 12;
    outLineRadius = 75;
    innLineRadius = 60;
    bgERadius = 45;
    textOutRadius = 30;
    textInnRadius = 20;
}

Dashboard::~Dashboard()
{
    delete ui;
}

void Dashboard::paintEvent(QPaintEvent *event){//界面绘制事件
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);//设置渲染,启动反锯齿
    painter.translate(width / 2, height / 2);//平移坐标系到窗口正中央
    painter.scale(side / 200.0, side / 200.0);//绘制的图片对窗口进行自适应

    drawBGE(&painter); //画背景圆
    drawTextE(&painter);//画文字所在圆弧
    drawText(&painter);//画文字
    drawLines(&painter);//画最外部的线
}

void Dashboard::updateValue(double value){//提供给外部的方法，用于更新value
    this->value = value;
    update();//每次更新value后，都重绘界面
}

void Dashboard::drawLines(QPainter* painter){
    painter->save();
    QPen pen;
    //pen.setColor(QColor("#A7DD42"));//初始化默认所有的线都应该显示（100%）
    //pen.setColor(QColor("#a8ff60"));
    pen.setColor(QColor("#F1FA02"));
    pen.setWidth(2);//改变色彩的深浅
    painter->setPen(pen);

    double range = 270.0/lineCount; //自定义所有的线加起来占270度，100条线
    painter->rotate(135);//旋转135度，开始画线，总共画270度。
    QLine line(QPoint(outLineRadius,0),QPoint(innLineRadius,0));//最外圈的小横线，100条，按照坐标从outLineRadius到innLineRadius
    for(int i = 1;i<=lineCount;i++){
        QPen pen;
        pen.setWidth(2);
        if(i>60&&i<=value){
//            QPen pen;
            pen.setColor(QColor("#FF8C00"));
//            pen.setWidth(2);
            painter->setPen(pen);
        }
        if(i>90&&i<=value){
//            QPen pen;
            pen.setColor(QColor("#FF0107"));
//            pen.setWidth(2);
            painter->setPen(pen);
        }
        if(i>value){//把应该不显示的部分变成灰色（按照中间的百分比数）
//            QPen pen;
            pen.setColor(QColor("#D7D7D7"));
//            pen.setWidth(2);
            painter->setPen(pen);
        }
        painter->drawLine(line);
        painter->rotate(range);
    }
    painter->restore();
}

void Dashboard::drawBGE(QPainter* painter){
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#EAEAEA"));
    QRect rect(-bgERadius,-bgERadius,bgERadius*2,bgERadius*2);
    painter->drawEllipse(rect);//画出背景圆(灰色)
    painter->restore();
}

void Dashboard::drawTextE(QPainter* painter){
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#DEB887"));//画出中间的绿色圆环

    QPainterPath path;
    QRect rectOut(-textOutRadius,-textOutRadius,textOutRadius*2,textOutRadius*2);
    path.arcTo(rectOut,0,360);//画出圆环外围

    QPainterPath subPath;
    QRect rectInn(-textInnRadius,-textInnRadius,textInnRadius*2,textInnRadius*2);
    subPath.addEllipse(rectInn);//内芯
    path -= subPath;
    painter->drawPath(path);
    painter->restore();
}

void Dashboard::drawText(QPainter* painter){
    painter->save();
    painter->setPen(QColor("#62A0DB"));
    QRect rectInn(-textInnRadius,-textInnRadius,textInnRadius*2,textInnRadius*2);//???????
    QFont font = painter->font();
    font.setPixelSize(textSize);//设置字体大小
    font.setFamily("Microsoft YaHei");
    painter->setFont(font);
    painter->drawText(rectInn,Qt::AlignCenter,QString::number(value)+"%");
    painter->restore();
}

void Dashboard::delValue()
{
    value--;
    updateValue(value);
}

void Dashboard::addValue()
{
    value++;
    updateValue(value);
}

int Dashboard::getValue() const
{
    return value;
}
