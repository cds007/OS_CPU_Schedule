#ifndef WIDGET_H
#define WIDGET_H
#define NUM_OF_PROCESS 6
#define TIME_SLICE 10

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QPushButton>
#include <QTimer>
#include "sprocess.h"
#include "backupque.h"
#include "addworks.h"
#include "readyque.h"
#include "selectsuspend.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

signals:
    void upmoveEnd();
    void cpuwaitEnd();
    void downmoveEnd();

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void initTimers();
    void initSustasks();
    void stopTimers();
    void addProcess();
    void addIntoReady(SProcess *procs);
    void addIntoReadyWithSlice(SProcess *procs);
    void startAddProcsInit();
    void addMultiTasks(QSqlQuery &result);

    SProcess *emptyProcs();

    QSqlDatabase getDB();
    int countBackQue();
    void delFirstOfDB();
    void initMoveBtn_1();
    void upBtn_Move_1();
    void downBtn_Move_1();
    void downBtn_Move_1_withslice();
//    void downBtn_Move_1_withoutslice();
    void dataintoMovebtn();
    void stopinCPU();
    void delMoveProcsTime();
    void CPUscheduling();

    void changeUpXY();
    void changeDownXY();
    void changeDownXYWithSlice();
    void cpuReceivedtoWait();
    void cpuIfKeepWait();
    void cpuIfKeepWaitWithSlice();
    void workScheduling();

    void addSuspendQue(SProcess *select);
    void delSuspendQue(SProcess *del);
    void refreshSuspendQue();
    SProcess *searchProcsInSuspdQue(QString PID, int pri, int tim);

    void crushRunningProcs();

private slots:
    void on_AddWork_clicked();

    void on_ViewBackQue_clicked();

    void timerEvent(QTimerEvent *eve) Q_DECL_OVERRIDE;

    void on_Start_clicked();

    void on_Terminate_clicked();

    void on_Random_clicked();

    void on_continue_2_clicked();

    void on_Suspend_clicked();

    void addtoSuspendQue(SProcess *receive);

    void on_DeSuspend_clicked();

private:
    Ui::Widget *ui;
    BackUpQue *backque;
    ReadyQue *readyque;
    SelectSuspend *selesuspd;//展示可以挂起的进程
    SProcess *moveProcs;//存储移动的btn所代表的进程对象的信息
    int timerMonitNum;
    int timerstartall;
    int timerup;
    int timerdown;
    int timercpuwait;
    int timercpudeltime;
    int timerdownwithslice;
    //各个计时器

    SProcess multitasks[NUM_OF_PROCESS];



    SProcess *sustasks[NUM_OF_PROCESS];
    int num_in_memory;//内存中的总进程数
    int num_in_suspend;//被挂起的总进程数
    int curRunTime;//记录运行时间：时间片
    int runModel;
    bool canPopReady;//ready队列是否可以弹出一个优先级最高的进程
    bool canUpMove;//上半移动是否可以开始
    bool cpuWait;//cpu处理等待是否开始
    bool canDownMove;//下半移动是否可以开始
    bool canDownMoveWithSlice;
    bool ifcpuWaitover;//cpu等待是否完毕
    bool ifCPUschedule;//是否可以进行一次cpu调度循环
    bool ifCPUscheduleIsRunning;//cpu调度循环是否正在进行

    int timercrush;
};

#endif // WIDGET_H
