#ifndef ALLPCBCONTROLLER_H
#define ALLPCBCONTROLLER_H

#include<iostream>
#include<cstdio>
#include<QVector>
#include<cmath>
#include<QDebug>
#include<algorithm> //是c++特有的STL模板的算法头文件 包含了一些特定的算法函数 ,如sort()
#include "pcb.h"
#include "memoryitem.h"
#include "queuedao.h"

using namespace std;

class AllPcbController
{
public:
    AllPcbController();//构造函数

    bool creatProcess(PCB p);//创建新的进程
    bool deleteWaiting(QString pName);//删除waiting里的一个进程

    void SortReadyList();//就绪队列排序
    bool transferWaitingToReady(int n);//转运waiting到ready
    bool transferWaitingToReadyBest(int n);//最优适应算法
    bool transferWaitingToReadyWorst(int n);//最差适应算法
    bool ReadyChangeToRunning();//ready到running
    bool ReadyChangeToRunningTimer();//ready到running之时间片,//判断running上的进程是否要下CPU了，通过增加已运行时间判断
    bool RunningToTerminate();//runninng到terminate
    bool ReadyToHanging(int pos);//ready到hanging
    bool HangingToWaiting(QString pname);//hanging到waiting的队首
    bool aging();//老化
    bool agingTimer();//老化
    void reStart();//重新开始

    bool AddToMemory(PCB* p);//判断这个进程是否能被加入到主存中去
    bool AddToMemoryBest(PCB* p);//最优适应算法
    bool AddToMemoryWorst(PCB* p);//最差适应算法
    bool reclaimMemory(PCB* p);//通过查pcb名称的方式来删除其在主存中的位置

    QVector<PCB> getExistPcbList() const;

    QVector<PCB> getWaitingList() const;

    QVector<PCB> getReadyList() const;

    QVector<PCB> getRunningList() const;

    QVector<PCB> getTerminateList() const;

    QVector<PCB> getHangingList() const;

    QVector<MemoryItem> getMemoryList() const;


private:
    QVector<PCB> ExistPcbList;//目前存在的所有进程的队列，包括以下三个
    QVector<PCB> WaitingList;
    QVector<PCB> ReadyList;
    QVector<PCB> RunningList;
    QVector<PCB> TerminateList;//终止队列
    QVector<PCB> HangingList;//挂起队列
    QVector<MemoryItem> MemoryList;//主存队列
    QueueDao *queueDao;//数据库指针

    PCB osPcb;
    const int osSize=10;//操作系统大小
    const int memorySize=63;//设置主存最后一个地址，主存大小是64
    int timeslice=5;//时间片

};

#endif // ALLPCBCONTROLLER_H
