#include "allpcbcontroller.h"

AllPcbController::AllPcbController()
{
  //创建并打开数据库
  queueDao = new QueueDao();
  queueDao->openDb();
  // 判断数据表是否存在
  QString str1 = QString("waitingQueue");
  qDebug() << "isTabelExist：" <<queueDao->isTableExist(str1);
  //初始化数据
  queueDao->queryTable(ExistPcbList);
  queueDao->queryTable(WaitingList);
  qDebug()<<"waiting的长度是 "<<WaitingList.length();

  //初始化主存队列
  MemoryItem os("操作系统",0,10,1);
  MemoryList.append(os);
}

bool AllPcbController::creatProcess(PCB p){
    QVector<PCB>::iterator iter;
    for (iter=ExistPcbList.begin();iter!=ExistPcbList.end();iter++){
        if (QString::compare(iter->getPcbName(),p.getPcbName())==0){
            return false;//查找是否有名字相等的pcb
        }
    }
    ExistPcbList.append(p);
    WaitingList.append(p);
    queueDao->singleInsertData(p);//插入数据库中

    return true;
}

bool AllPcbController::deleteWaiting(QString pName)
{
    if (WaitingList.isEmpty()){
        return false;
    }else{
        for (int i=0;i<ExistPcbList.length();i++) {
            if (QString::compare(ExistPcbList[i].getPcbName(),pName)==0){
                ExistPcbList.remove(i);
                //return true;
            }
        }
        for (int i=0;i<WaitingList.length();i++) {
            if (QString::compare(WaitingList[i].getPcbName(),pName)==0){
                WaitingList.remove(i);
                queueDao->deleteData(pName);
                return true;
            }
        }
    }
    return false;
}

bool AllPcbController::transferWaitingToReady(int n)
{
    int cnt=0;
    if (WaitingList.length()==0){
        return false;
    }else{
        QVector<PCB> wait=WaitingList;//很重要，先拷贝waitinglist
        for (int i=0;i<wait.length();i++) {
            //在主存中应该是没有按运行时间排序的
            //操作系统首先从外存的后备队列中选取某些作业调入内存，并为它们创建进程、分配必要的资源。然后再将新创建的进程插入就绪队列，准备执行。
            PCB *pM = &(wait[i]);
            if (AddToMemory(pM)){
                //成功插入
                ReadyList.append(wait.at(i));
                //修改状态，先放在这里，如果需要的话再想办法修改
                ReadyList[cnt++].setState("ready");
                //移除waitinglist
                for (int j=0;j<WaitingList.length();j++){
                    if (QString::compare(WaitingList[j].getPcbName(),wait.at(i).getPcbName())==0){
                        qDebug()<<"成功移除!";
                        WaitingList.remove(j);
                        break;
                    }
                }
                //暂时先不删数据库
                //queueDao->deleteData(wait.at(i).getPcbName());//数据库也删掉
                if (cnt==n){
                    return true;
                }
                if (WaitingList.length()==0){
                    break;
                }
            }
        }
    }
    qDebug()<<ReadyList.length()<<"in transferWaitingToReady";
    qDebug()<<"can not insert because no enough momory!";
    return false;
}

bool AllPcbController::transferWaitingToReadyBest(int n)
{
    int cnt=0;
    if (WaitingList.length()==0){
        return false;
    }else{
        QVector<PCB> wait=WaitingList;//很重要，先拷贝waitinglist
        for (int i=0;i<wait.length();i++) {
            //在主存中应该是没有按运行时间排序的
            //操作系统首先从外存的后备队列中选取某些作业调入内存，并为它们创建进程、分配必要的资源。然后再将新创建的进程插入就绪队列，准备执行。
            PCB *pM = &(wait[i]);
            if (AddToMemoryBest(pM)){
                //成功插入
                ReadyList.append(wait.at(i));
                //修改状态，先放在这里，如果需要的话再想办法修改
                ReadyList[cnt++].setState("ready");
                //移除waitinglist
                for (int j=0;j<WaitingList.length();j++){
                    if (QString::compare(WaitingList[j].getPcbName(),wait.at(i).getPcbName())==0){
                        qDebug()<<"成功移除!";
                        WaitingList.remove(j);
                        break;
                    }
                }
                //暂时先不删数据库
                //queueDao->deleteData(wait.at(i).getPcbName());//数据库也删掉
                if (cnt==n){
                    return true;
                }
                if (WaitingList.length()==0){
                    break;
                }
            }
        }
    }
    qDebug()<<ReadyList.length()<<"in transferWaitingToReadyBest";
    qDebug()<<"can not insert because no enough momory!";
    return false;
}

bool AllPcbController::transferWaitingToReadyWorst(int n)
{
    int cnt=0;
    if (WaitingList.length()==0){
        return false;
    }else{
        QVector<PCB> wait=WaitingList;//很重要，先拷贝waitinglist
        for (int i=0;i<wait.length();i++) {
            //在主存中应该是没有按运行时间排序的
            //操作系统首先从外存的后备队列中选取某些作业调入内存，并为它们创建进程、分配必要的资源。然后再将新创建的进程插入就绪队列，准备执行。
            PCB *pM = &(wait[i]);
            if (AddToMemoryWorst(pM)){
                //成功插入
                ReadyList.append(wait.at(i));
                //修改状态，先放在这里，如果需要的话再想办法修改
                ReadyList[cnt++].setState("ready");
                //移除waitinglist
                for (int j=0;j<WaitingList.length();j++){
                    if (QString::compare(WaitingList[j].getPcbName(),wait.at(i).getPcbName())==0){
                        qDebug()<<"成功移除!";
                        WaitingList.remove(j);
                        break;
                    }
                }
                //暂时先不删数据库
                //queueDao->deleteData(wait.at(i).getPcbName());//数据库也删掉
                if (cnt==n){
                    return true;
                }
                if (WaitingList.length()==0){
                    break;
                }
            }
        }
    }
    qDebug()<<ReadyList.length()<<"in transferWaitingToReadyWorst";
    qDebug()<<"can not insert because no enough momory!";
    return false;
}

void AllPcbController::SortReadyList()
{
    for (int i=0;i<ReadyList.length()-1;i++) {
        int k=i;
        for (int j=i+1;j<ReadyList.length();j++) {
            if (ReadyList.at(j).getPriority()>ReadyList.at(k).getPriority()){
                k=j;
            }
        }
        if (k!=i){
            PCB p1=ReadyList[i];
            ReadyList[i]=ReadyList[k];
            ReadyList[k]=p1;
        }
    }
    for (int i=0;i<ReadyList.length();i++) {
        qDebug()<<ReadyList[i].getPcbName()<<" "<<ReadyList[i].getPriority();
    }
}

bool AllPcbController::ReadyChangeToRunning()
{
      //这里实现抢占，先ready和running有没有，再判断readylist的第一个和running的第一个谁优先级高，谁上CPU
    if (ReadyList.length()==0){
        return true;
    }
    if (RunningList.length()==0){
        //runninglist无数据
        RunningList.append(ReadyList[0]);
        ReadyList.remove(0);
        return true;
    }else{
        if (ReadyList[0].getPriority()>RunningList[0].getPriority()){
            //要抢占了,互相交换
            PCB p1=ReadyList[0];
            ReadyList[0]=RunningList[0];
            RunningList[0]=p1;
            SortReadyList();//ReadyList重新排序
            return false;
        }
    }
    return true;
}

bool AllPcbController::ReadyChangeToRunningTimer()
{
    if (ReadyList.length()==0){
        return true;
    }
    if (RunningList.length()==0){
        //runninglist无数据
        RunningList.append(ReadyList[0]);
        ReadyList.remove(0);
        return true;
    }else{
        //当时间片到了就抢占
        if (RunningList[0].getHasRunning()>=timeslice){
            RunningList[0].setHasRunning(0);//先重置运行时间
            PCB p1=ReadyList[0];
            ReadyList.remove(0);//把ready队首移除
            ReadyList.append(RunningList[0]);//放在队末
            RunningList[0]=p1;
            return false;
        }
    }
    return true;
}

bool AllPcbController::RunningToTerminate()
{
    if (RunningList[0].getPcbTime()!=0){
        qDebug()<<"时间不为0不可终止";
        return false;
    }
    PCB P1=RunningList[0];
    //从内存中删去
    PCB *pM =&P1;
    reclaimMemory(pM);
    TerminateList.append(P1);
    RunningList.remove(0);
    for(int i=0;i<ExistPcbList.length();i++){
        if (QString::compare(ExistPcbList[i].getPcbName(),P1.getPcbName())==0){
            ExistPcbList.remove(i);
        }
    }
    return true;
}

bool AllPcbController::ReadyToHanging(int pos)
{
    //这个也得移除内存
    //pos传进来是0,1,2,3
    if (pos<0||pos>3){
        return false;
    }
    if (ReadyList.length()<=pos){
        //长度不够，出错
        qDebug()<<"ReadyList 长度不够，此错误在CONTROLLER中";
        return false;
    }else{
        PCB p1=ReadyList[pos];
        PCB* pM =&p1;
        reclaimMemory(pM);
        HangingList.append(p1);
        ReadyList.remove(pos);
        return true;
    }
}

bool AllPcbController::HangingToWaiting(QString pname)
{
    for (int i=0;i<HangingList.length();i++) {
        if (QString::compare(HangingList[i].getPcbName(),pname)==0){
            PCB p1=HangingList[i];
            WaitingList.insert(0,p1);
            HangingList.remove(i);
            qDebug()<<"HangingToWaiting移动成功，在CONTROLLER";
            return true;
        }
    }
    qDebug()<<"HangingToWaiting移动失败，在CONTROLLER,可能没找到这个进程";
    return false;
}

bool AllPcbController::aging()
{
    if (RunningList.length()==0){
        qDebug()<<"running 没了";
        return false;
    }else{
        int p=(RunningList[0].getPriority())-1;
        if (p<1){
            p=1;//优先级最低只能是1
        }
        RunningList[0].setPriority(p);
        int t=(RunningList[0].getPcbTime())-1;
        RunningList[0].setPcbTime(t);
        qDebug()<<"老化后的时间是："<<RunningList[0].getPcbTime();
        int h=(RunningList[0].getHasRunning())+1;//已运行时间加一
        RunningList[0].setHasRunning(h);
        if (t<=0){
            //运行时间到了，去终止队列
            return false;
        }
    }
    qDebug()<<"设置完之后的时间:"<<RunningList[0].getPcbTime();
    return true;
}

bool AllPcbController::agingTimer()
{
    if (RunningList.length()==0){
        qDebug()<<"running 没了";
        return false;
    }else{
        int t=(RunningList[0].getPcbTime())-1;
        RunningList[0].setPcbTime(t);
        int h=(RunningList[0].getHasRunning())+1;//已运行时间加一
        RunningList[0].setHasRunning(h);
        if (t<=0){
            //运行时间到了，去终止队列
            return false;
        }
    }
    qDebug()<<"设置完之后的时间:"<<RunningList[0].getPcbTime();
    return true;
}



void AllPcbController::reStart()
{
    WaitingList.clear();
    ExistPcbList.clear();
    ReadyList.clear();
    RunningList.clear();
    HangingList.clear();
    TerminateList.clear();
    MemoryList.clear();
    //初始化主存队列
    MemoryItem os("操作系统",0,10,1);
    MemoryList.append(os);
    //读数据库
    queueDao->queryTable(ExistPcbList);
    queueDao->queryTable(WaitingList);
    qDebug()<<"waiting的长度是 "<<WaitingList.length();
}

bool AllPcbController::AddToMemory(PCB* p)
{
    for(int i=0;i<MemoryList.length();i++){
        //遍历主存list
        if (i!=MemoryList.length()-1){
            //不是最后一个
            if ((MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength())<MemoryList.at(i+1).getStartpos()){
                //中间有空闲
                if ((MemoryList.at(i+1).getStartpos()-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength()))>=p->getPcbSize()){
                    //可用长度大于等于pcb所需大小
                    p->setPcbStartPos(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength());
                    MemoryItem memoryPcb(p->getPcbName(),p->getPcbStartPos(),p->getPcbSize(),1);
                    MemoryList.insert(i+1,memoryPcb);//插入到i+1的位置，不确定，先看看
                    qDebug()<<"插入成功！";
                    return true;
                }
            }
        }else{
            //是最后一个
            if ((memorySize-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength())+1)>=p->getPcbSize()){
                //可以直接后插
                p->setPcbStartPos(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength());
                MemoryItem memoryPcb(p->getPcbName(),p->getPcbStartPos(),p->getPcbSize(),1);
                MemoryList.append(memoryPcb);
                qDebug()<<"后插成功！";
                return true;
            }
        }
    }
    qDebug()<<"无合适位置插入！";
    return false;
}

bool AllPcbController::AddToMemoryBest(PCB *p)
{
    int minLength = 65535;
    int minPoint = 65535;
    for(int i=0;i<MemoryList.length();i++){
        //遍历主存list
        if (i!=MemoryList.length()-1){
            //不是最后一个
            if ((MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength())<MemoryList.at(i+1).getStartpos()){
                //中间有空闲
                if ((MemoryList.at(i+1).getStartpos()-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength()))>=p->getPcbSize()){
                    //可用长度大于等于pcb所需大小
                    if ((MemoryList.at(i+1).getStartpos()-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength()))<minLength){
                        minLength=(MemoryList.at(i+1).getStartpos()-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength()));
                        minPoint = i+1;
                    }
                }
            }
        }else{
            //是最后一个
            if ((memorySize-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength())+1)>=p->getPcbSize()){
                //可以直接后插
                if ((memorySize-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength())+1)<minLength){
                    minLength=(memorySize-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength())+1);
                    minPoint = i+1;
                }
            }
        }
    }
    if (minPoint==MemoryList.length()){
        //说明是最后一个
        int i = minPoint-1;
        p->setPcbStartPos(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength());
        MemoryItem memoryPcb(p->getPcbName(),p->getPcbStartPos(),p->getPcbSize(),1);
        MemoryList.append(memoryPcb);
        qDebug()<<"后插成功！";
        return true;
    }else if (minLength!=65535){
        //说明找到了是中间的
        int i =minPoint-1;
        p->setPcbStartPos(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength());
        MemoryItem memoryPcb(p->getPcbName(),p->getPcbStartPos(),p->getPcbSize(),1);
        MemoryList.insert(i+1,memoryPcb);//插入到i+1的位置，不确定，先看看
        qDebug()<<"插入成功！";
        return true;
    }else{
        qDebug()<<"无合适位置插入！";
        return false;
    }
}

bool AllPcbController::AddToMemoryWorst(PCB *p)
{
    int maxLength = 0;
    int maxPoint = 65535;
    for(int i=0;i<MemoryList.length();i++){
        //遍历主存list
        if (i!=MemoryList.length()-1){
            //不是最后一个
            if ((MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength())<MemoryList.at(i+1).getStartpos()){
                //中间有空闲
                if ((MemoryList.at(i+1).getStartpos()-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength()))>=p->getPcbSize()){
                    //可用长度大于等于pcb所需大小
                    if ((MemoryList.at(i+1).getStartpos()-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength()))>maxLength){
                        maxLength=(MemoryList.at(i+1).getStartpos()-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength()));
                        maxPoint= i+1;
                    }
                }
            }
        }else{
            //是最后一个
            if ((memorySize-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength())+1)>=p->getPcbSize()){
                //可以直接后插
                if ((memorySize-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength())+1)>maxLength){
                    maxLength=(memorySize-(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength())+1);
                    maxPoint = i+1;
                }
            }
        }
    }
    if (maxPoint==MemoryList.length()){
        //最后一个可行
        int i = maxPoint-1;
        p->setPcbStartPos(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength());
        MemoryItem memoryPcb(p->getPcbName(),p->getPcbStartPos(),p->getPcbSize(),1);
        MemoryList.append(memoryPcb);
        qDebug()<<"后插成功！";
        return true;
    }else if (maxLength!=0){
        //插中间
        int i = maxPoint-1;
        p->setPcbStartPos(MemoryList.at(i).getStartpos()+MemoryList.at(i).getLength());
        MemoryItem memoryPcb(p->getPcbName(),p->getPcbStartPos(),p->getPcbSize(),1);
        MemoryList.insert(i+1,memoryPcb);//插入到i+1的位置，不确定，先看看
        qDebug()<<"插入成功！";
        return true;
    }else{
        qDebug()<<"无合适位置插入！";
        return false;
    }
}

bool AllPcbController::reclaimMemory(PCB* p)
{
    for (int i=0;i<MemoryList.length();i++) {
        //QString::compare(WaitingList[i].getPcbName(),pName)==0
        if (QString::compare(MemoryList[i].getPid(),p->getPcbName())==0){
            //找到pcb
            p->setPcbStartPos(-1);
            MemoryList.remove(i);
            return true;
        }
    }
    return false;
}

QVector<PCB> AllPcbController::getExistPcbList() const
{
    return ExistPcbList;
}

QVector<PCB> AllPcbController::getWaitingList() const
{
    return WaitingList;
}

QVector<PCB> AllPcbController::getReadyList() const
{
    return ReadyList;
}

QVector<PCB> AllPcbController::getRunningList() const
{
    return RunningList;
}

QVector<PCB> AllPcbController::getTerminateList() const
{
    return TerminateList;
}

QVector<PCB> AllPcbController::getHangingList() const
{
    return HangingList;
}

QVector<MemoryItem> AllPcbController::getMemoryList() const
{
    return MemoryList;
}




