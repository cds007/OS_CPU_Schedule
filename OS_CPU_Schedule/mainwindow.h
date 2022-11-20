#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTextCodec>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QStateMachine>
#include <QState>
#include <QSignalTransition>
#include <QTimer>
#include <QTime>
#include <ui_addnewprocessdialog.h>
#include "addnewprocessdialog.h"
#include "allpcbcontroller.h"
#include "usingways.h"
#include "dashboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void showTableViewHead();//显示表头

    void createAllConnect();//连接一些信号与槽

    void setFirstButton();//设置新建的那个按钮

private slots:
    void on_AddProcess_pushButton_clicked();//添加进程按钮的槽函数

    void reWaiting();//重置waiting队列

    void on_clearWaiting_pushButton_clicked();

    void showReadyList();//显示ready队列的内容

    void showTerminate();//显示终止队列的内容

    void showHanging();//显示挂起队列

    void showMemory();//显示主存表格内容

    void GoToRunning();//去Running的动画

    void on_pushButton_4_clicked(bool checked);

    void on_pushButton_3_clicked(bool checked);

    void on_pushButton_2_clicked(bool checked);

    void on_pushButton_clicked(bool checked);

    void on_Start_pushButton_clicked();

    void on_Next_pushButton_clicked();

    void on_hanging_pushButton_clicked();

    void on_relieveHanging_pushButton_clicked();

    void on_hanging_tableView_clicked(const QModelIndex &index);

    void on_restart_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    AddNewProcessDialog *addnewprocessdialog;//添加进程界面指针
    AllPcbController *PcbController;//添加一个管理PCB类的指针

    QStandardItemModel* m_tableModel;//tableview里的model指针
    QStandardItemModel* t_tableModel;//tableview里的model指针
    QStandardItemModel* h_tableModel;//tableview里的model指针
    QStandardItemModel* memory_tableModel;//主存的model指针
    QPushButton FirstButton;//创建第五个按钮
    Dashboard *dashboard;//仪表盘指针

    QString cell;
    int HangCurRow;
    QPropertyAnimation *m_animation;
    QTimer *tim;
    bool TimerIsChange=false;//判断用的是不是时间片轮转调度算法
    bool PriIsChange=false;//判断用的是不是优先级调度算法
    bool Auto=false;//是否正在自动
    bool isCanStart=true;//是否能点开始

    bool FIFOFit=false;//判断用的是不是首次适应算法
    bool BestFit=false;//判断用的是不是最优适应算法
    bool WorstFit=false;//判断用的是不是最差适应算法
};

#endif // MAINWINDOW_H
