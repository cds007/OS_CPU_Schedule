#include "widget.h"
#include "ui_widget.h"
#define random(a,b) (rand() % (b-a+1))+a

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    num_in_memory = 0;
    curRunTime = 0;
    num_in_suspend = 0;
    runModel = 0;//初始默认纯优先级
    ui->status_2->setVisible(false);
    moveProcs = nullptr;

    //一些判断标准
    canPopReady = false;//ready队列不能pop->可能是：正在upmove或者队列为空，前期默认不能
    canUpMove = false;
    canDownMove = false;
    canDownMoveWithSlice = false;
    cpuWait = false;//cpu处于未进入等待状态
    ifcpuWaitover = true;//一开始cpu处于等待完毕状态，cpu对move的进程的时间--不会发生
    ifCPUschedule = false;//CPU调度尚未开始
    ifCPUscheduleIsRunning = false;//cpu调度未正在进行



    srand((int)time(NULL));//PID随机数种子init


    initMoveBtn_1();
    initSustasks();//初始化挂起数组

    backque = new BackUpQue();
    readyque = new ReadyQue(this);
    readyque->raise();
    readyque->show();
    selesuspd = new SelectSuspend;
    selesuspd->hide();

    //开始将进程加入到ready队列中
    startAddProcsInit();

    connect(selesuspd, SIGNAL(selectok(SProcess*)), this, SLOT(addtoSuspendQue(SProcess*)) );
    //选中挂起的进程连接槽函数
}

Widget::~Widget()
{
    delete ui;
    if(backque!=nullptr)  delete backque;
    if(readyque!=nullptr) delete readyque;
    if(selesuspd!=nullptr) delete selesuspd;
}

/**
 * @brief Widget::initTimers
 * 初始化（开始）计时器
 * 纯优先级、纯时间片均可用
 */
void Widget::initTimers()
{
    timerstartall = startTimer(5);
    timerup = startTimer(3);
    timerdown = startTimer(3);
    timercpuwait = startTimer(5);
    timercpudeltime = startTimer(500);
    timerMonitNum = startTimer(1500);//添加进程至ready队列中
    timerdownwithslice = startTimer(5);

    timercrush = startTimer(5);
}

/**
 * @brief Widget::initSustasks
 * 初始化挂起队列
 * 纯优先级、纯时间片均可用
 */
void Widget::initSustasks()
{
    for(int i=0;i<NUM_OF_PROCESS;i++)
        sustasks[i] = nullptr;
}

/**
 * @brief Widget::stopTimers
 * 终止计时器
 * 纯优先级、纯时间片均可用
 */
void Widget::stopTimers()
{
    this->killTimer(timerstartall);
    this->killTimer(timerup);
    this->killTimer(timerdown);
    this->killTimer(timercpuwait);
    this->killTimer(timercpudeltime);
    this->killTimer(timerMonitNum);
    this->killTimer(timerdownwithslice);

    this->killTimer(timercrush);
}
//关键函数
/**
 * 选取后备队列第一个往内存新加一个进程（然后进ready队列）:
 * 1、进主界面的multitasks[]数组
 * 2、进ready队列
 * 每选一个，填入一个SProcess数组元素（改变状态值），删除头一条数据库记录
 * 纯优先级、纯时间片均可用
 * @brief Widget::addProcess
 */
void Widget::addProcess()
{
    if(num_in_memory < NUM_OF_PROCESS && num_in_memory>=0)
    {
        QSqlDatabase db = getDB();
        bool ok = db.open();
        if(ok){
            QSqlQuery result = db.exec(QString("SELECT pname,priority,runtime"
                                               " FROM backupque ORDER BY id ASC LIMIT 1"));
            while (result.next()) {
                addMultiTasks(result);//新加进主界面的multitasks[]数组和ready队列
                num_in_memory += 1;
            }
            db.close();
            delFirstOfDB();//删除头一条记录
            backque->refreshAgain();
        }
        else {
           qDebug()<< "Database open failed in addProcess() ";
        }
    }
    else {
        qDebug()<< "number of processes is limited in addProcess()";
        return;
    }
}

/**
 * @brief Widget::addIntoReady
 * 将信息做成指针存进ready(自动刷新ready展示的按钮)
 * 纯优先级均可用
 */
void Widget::addIntoReady(SProcess *procs)
{
    readyque->ReceiveReadyProcs(procs);
}

/**
 * @brief Widget::addIntoReady
 * 将信息做成指针存进ready(自动刷新ready展示的按钮)
 * 纯时间片均可用
 */
void Widget::addIntoReadyWithSlice(SProcess *procs)
{
    readyque->ReceiveReadyProcsWithSlice(procs);
}

/**
 * 开始初始化：
 * 1、如果后备队列中个数>=6，则从后备队列中按照id的顺序（FCFS）选取6个进内存（ready）
 * 2、如果后备队列中个数<6，则将后备队列中所有进内存（ready）
 * for循环每选一个，填入一个SProcess数组元素（改变状态值），删除一条记录
 * 纯优先级、纯时间片均可用
 * @brief Widget::startAddProcsInit
 */
void Widget::startAddProcsInit()
{
    if(num_in_memory == 0)
    {
        QSqlDatabase db = getDB();
        bool ok = db.open();
        if(ok){
            if(countBackQue()>=6)
            {
                //选最前面的6个（按照FCFS）
                for(int i=0;i<NUM_OF_PROCESS;i++)
                {
                    addProcess();
                }

            }
            else {
                int num = countBackQue();
                for(int i=0;i<num;i++)
                {
                    qDebug()<<countBackQue();
                    addProcess();
                }
            }
        }
        else {
           qDebug()<< "Database open failed in startAddProcsInit()";
           db.close();
        }
    }
    else {
        qDebug()<< "number of processes is not 0 in startAddProcsInit()33";
        return;
    }
}

/**
 * @brief Widget::addMultiTasks
 * @param result
 * 将信息填入一个multitasks数组元素（进程）
 * 纯优先级、纯时间片均可用
 */
void Widget::addMultiTasks(QSqlQuery &result)
{
    SProcess *p = emptyProcs();
    p->setPname(result.value("pname").toString());
    p->setPstatus(READY);
    p->setPriority(result.value("priority").toInt());
    p->setRunTime(result.value("runtime").toInt());
    p->setDefaultPCB();//设置pname、status、priority、runtime
    p->getPCB()->setPID(random(1000,9999));

    addIntoReady(p);//新加进ready队列
//    multitasks[num_in_memory].setPname(result.value("pname").toString());
//    multitasks[num_in_memory].setPstatus(READY);
//    multitasks[num_in_memory].setPriority(result.value("priority").toInt());
//    multitasks[num_in_memory].setRunTime(result.value("runtime").toInt());
//    multitasks[num_in_memory].setDefaultPCB();//设置pname、status、priority、runtime
//    multitasks[num_in_memory].getPCB()->setPID(random(1000,9999));
}

/**
 * @brief Widget::emptyProcs
 * @return 返回可用的进程对象数组空间
 * 纯优先级、纯时间片均可用
 */
SProcess *Widget::emptyProcs()
{
    if(num_in_memory<NUM_OF_PROCESS){
        for(int i=0;i<NUM_OF_PROCESS;i++){
            if(multitasks[i].getPCB()->getStatus() == BACKUP||
                    multitasks[i].getPCB()->getStatus() == TERMINATED)
            {
                return &multitasks[i];
            }
        }
    }
}

/**
 * @brief Widget::getDB
 * @return 返回一个既定的QSqlDatabase对象简化代码
 * 纯优先级、纯时间片均可用
 */
QSqlDatabase Widget::getDB()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("mysql"))
    {
        db = QSqlDatabase::database("mysql");
    }
    else {
        db = QSqlDatabase::addDatabase("QMYSQL","mysql");
    }
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("cpuschedule");
    db.setUserName("root");
    db.setPassword("123456789");
    return db;
}

/**
 * @brief Widget::countBackQue
 * @return 统计后备队列中的个数
 * 纯优先级、纯时间片均可用
 */

//统计
int Widget::countBackQue()
{
    QSqlDatabase db = getDB();
    bool ok = db.open();
    int count;
    if(ok){
        QSqlQuery result = db.exec(QString("SELECT COUNT(*) FROM backupque"));
        while (result.next()) {
            count =  result.value("COUNT(*)").toInt();
            db.close();
            return count;
        }
    }
    else {
       qDebug()<< "Database open failed in countBackQue()";
       return 0;
    }
}

/*
 * 删除按照id（FCFS）顺序的第一条数据库记录
 * 纯优先级、纯时间片均可用
*/
void Widget::delFirstOfDB()
{
    QSqlDatabase db = getDB();
    bool ok = db.open();
    if(ok){
        QSqlQuery q(db);
        QSqlQuery result = db.exec(QString("SELECT id FROM backupque ORDER BY id ASC LIMIT 1"));
        while (result.next()) {
            QString del = QString("DELETE FROM backupque WHERE id=%1").arg(result.value("id").toString());
            bool scss = q.exec(del);
            if(scss)
                qDebug()<< "delete first success";
        }
    }
    else {
       qDebug()<< "Database open failed in delFirstOfDB()";
    }
    db.close();
    backque->initQueue();
}

/**
 * @brief Widget::initMoveBtn_1
 * 初始化移动按钮的位置和状态
 * 纯优先级、纯时间片均可用
 */
void Widget::initMoveBtn_1()
{
    ui->btn_move_1->setVisible(false);
    ui->btn_move_1->setText("");
    ui->btn_move_1->move(20,70);
}

/**
 * @brief Widget::upBtn_Move
 * 上半部分动画函数，从readyque后面把预先做好的button移出至CPU进行running
 * 纯优先级、纯时间片均可用
 */
void Widget::upBtn_Move_1()
{
    if(canUpMove){

        ui->btn_move_1->setVisible(true);
        changeUpXY();
    }
//    ui->btn_move_1->setVisible(true);
//    timerup->start(5);
//    return;
//    while(ui->btn_move_1->x()==400&&ui->btn_move_1->y()==220){
//        timerup->stop();
//        return;
//    }
}

/**
 * @brief Widget::downBtn_Move_1
 * 下半部分动画函数，从CPU把固定好的button移出至terminate
 * 纯优先级、纯时间片均可用
 */
void Widget::downBtn_Move_1()
{
    if(canDownMove){
        changeDownXY();
    }
//    timerdown->start(5);
//    return;
//    while(ui->btn_move_1->x()==280&&ui->btn_move_1->y()==380){
//        timerdown->stop();
//        ui->btn_move_1->setVisible(false);
//        return;
    //    }
}

/**
 * @brief Widget::downBtn_Move_1_withslice
 * 纯时间片可用
 */
void Widget::downBtn_Move_1_withslice()
{
    if(canDownMoveWithSlice){
        changeDownXYWithSlice();
    }
}

/**
 * @brief Widget::dataintoMovebtn
 * 将最高优先级Pop出ready队列，并接收其数据
 * 纯优先级专属
 */
void Widget::dataintoMovebtn()
{
    ui->status_2->setVisible(false);
    readyque->SortReadyProcs();//对ready队列排序
    moveProcs = readyque->PopReadyProcs();//从ready取出一个指针:优先级最高
    readyque->refreshAllbtns();//刷新ready的显示队列
    if(moveProcs!=nullptr)
    {
        canUpMove = true;
        ifCPUschedule = false;
        moveProcs->getPCB()->setStatus(MACRORUN);
        QString pri = QString::number(moveProcs->getPCB()->getPriority());
        ui->btn_move_1->setText("PID:"+moveProcs->getPCB()->getPID()+" 优先权:"+pri+" 时间:"+
                                moveProcs->getPCB()->getRuntime() );
        return;
    }
    else {
        qDebug()<< "ready is null";
        ifCPUschedule = false;
        return;
    }
}

/**
 * @brief Widget::stopinCPU
 * 纯优先级、纯时间片均可用
 */
void Widget::stopinCPU()
{
    if(cpuWait&&ifcpuWaitover)
    {
        ifcpuWaitover = false;
        //开始等待，计时器本来就一直在调用减时间函数，此时这个函数会响应，对该进程减时间
    }
}

/**
 * @brief Widget::delMoveProcsTime
 * 逐秒删除时间的函数，纯优先级可用
 */
void Widget::delMoveProcsTime()
{
    if(!ifcpuWaitover){
        //剩余时间减一
        moveProcs->getPCB()->delRuntime(1);
        moveProcs->setRunTime(moveProcs->getPCB()->getRuntime());
        moveProcs->getPCB()->delPriority(1);
        curRunTime++;//计录器加1

        //还在等待的进程优先权提升1
        readyque->addPriOfAllWait();

        //刷新按钮内容
        QString pri = QString::number(moveProcs->getPCB()->getPriority());
        QString tim = QString::number(moveProcs->getPCB()->getRuntime());
        ui->btn_move_1->setText("PID:"+moveProcs->getPCB()->getPID()+
                                " 优先权:"+pri+" 时间:"+tim);

        cpuIfKeepWaitWithSlice();//判断是否运行完毕
    }
}

/**
 * @brief Widget::CPUscheduling
 * CPU调度总函数，调用包括：
 * 1、从后备队列选一个进内存：ready =====>addProcess()
 * 2、从readyque选优先级最高的出来给move button =====>dataintoMovebtn()
 * 3、移动move button：上、下     =====>upBtn_Move_1()、downBtn_Move_1()
 * 4、处理完进程后进行判断：出内存
 * 纯优先级、纯时间片均可用
 */
void Widget::CPUscheduling()
{
    if(ifCPUschedule){//开始cpu调度循环
//        startAddProcsInit();
        ui->status->setText("调度中");
        ifCPUscheduleIsRunning = true;
        dataintoMovebtn();
    }
    else
        return;
}


/**
 * @brief Widget::on_xxx_clicked
 * 各类槽函数
 */

void Widget::on_AddWork_clicked()
{
    AddWorks *addwork = new AddWorks();
    addwork->show();
}

void Widget::on_ViewBackQue_clicked()
{
    backque->refreshAgain();
    backque->show();
}

void Widget::changeUpXY()
{
    int xnow = ui->btn_move_1->x();
    int ynow = ui->btn_move_1->y();
    if(xnow<400){
        ui->btn_move_1->move(xnow+1, ynow);
        return;
    }
    if(ynow<220){
        ui->btn_move_1->move(xnow, ynow+1);
        return;
    }
    else {
        canUpMove = false;
        cpuWait = true;//停止upmove，开始cpu等待运行时间为0的过程
        moveProcs->getPCB()->setStatus(RUNNING);
        ui->status->setText("处理中");
    }
}

void Widget::changeDownXY()
{
    int xnow = ui->btn_move_1->x();
    int ynow = ui->btn_move_1->y();
    if(ynow<460){
        ui->btn_move_1->move(xnow, ynow+1);
        return;
    }
    if(xnow>-270){
        ui->btn_move_1->move(xnow-1, ynow);
        return;
    }
    else {
        canDownMove = false;
        ifCPUschedule = true;//开启下一cpu调度循环
        ifCPUscheduleIsRunning = false;//cpu调度循环结束
        initMoveBtn_1();
    }
}

void Widget::changeDownXYWithSlice()
{
    int xnow = ui->btn_move_1->x();
    int ynow = ui->btn_move_1->y();
    if(ynow<380){
        ui->btn_move_1->move(xnow, ynow+1);
        return;
    }
    if(xnow>20){
        ui->btn_move_1->move(xnow-1, ynow);
        return;
    }
    else {
        canDownMoveWithSlice = false;
        ifCPUschedule = true;//开启下一cpu调度循环
        ifCPUscheduleIsRunning = false;//cpu调度循环结束
        addIntoReadyWithSlice(moveProcs);//进程进入ready队列
        initMoveBtn_1();
    }
}

/**
 * @brief Widget::cpuReceivedtoWait
 * 当cpu进入等待模式，调用stopinCPU()函数减时间
 */
void Widget::cpuReceivedtoWait()
{
    if(!cpuWait&&!canDownMove)//还在up移动
        return;
    else if(cpuWait&&!canDownMove)//up移动完毕，开始cpu等待
    {
        stopinCPU();
    }
    else
    {
        return;
    }
}

/**
 * @brief Widget::cpuIfKeepWait
 * cpu等待监测函数--不含时间片，纯优先权
 */
void Widget::cpuIfKeepWait()
{
    if(ifcpuWaitover)
        return;
    else {
        if(moveProcs->getPCB()->getRuntime()<=0){//运行完毕
            moveProcs->setPstatus(TERMINATED);
            moveProcs->getPCB()->setStatus(TERMINATED);//进程终止
            ifcpuWaitover = true;//结束减时间
            cpuWait = false;//结束stopinCPU()的调用
            canDownMove = true;//开始下半移动
            moveProcs = nullptr;
            num_in_memory--;
        }
    }
}

/**
 * @brief Widget::cpuIfKeepWaitWithSlice
 * cpu等待监测函数--纯时间片（也可附带优先级，因为这不是优先级的代表函数）
 */
void Widget::cpuIfKeepWaitWithSlice()
{
    if(ifcpuWaitover)
        return;
    else {
        if(curRunTime>=TIME_SLICE&&moveProcs->getPCB()->getRuntime()>0)//时间片限制时间到达
        {
            moveProcs->setPstatus(READY);
            moveProcs->getPCB()->setStatus(READY);
            curRunTime = 0;//清空运行时间记录器
            canDownMoveWithSlice = true;
            ifcpuWaitover = true;
            cpuWait = false;
            ui->status->setText("调度中");
        }
        if(moveProcs->getPCB()->getRuntime()<=0)//运行完毕
        {
            moveProcs->setPstatus(TERMINATED);
            moveProcs->getPCB()->setStatus(TERMINATED);//进程终止
            ifcpuWaitover = true;//结束减时间
            cpuWait = false;//结束stopinCPU()的调用
            canDownMove = true;//开始下半移动
            moveProcs = nullptr;
            curRunTime = 0;
            num_in_memory--;
            ui->status->setText("终止中");
        }
    }
}

/**
 * @brief Widget::workScheduling
 * 作业调度监测函数
 */
void Widget::workScheduling()
{
    if(num_in_memory < NUM_OF_PROCESS && countBackQue() > 0){
        addProcess();
    }
    else
        return;
}

/**
 * @brief Widget::addSuspendQue
 * @param select
 * 将目标指针加入挂起队列数组
 */
void Widget::addSuspendQue(SProcess *select)
{
    sustasks[num_in_suspend] = select;
    num_in_suspend++;
}

/**
 * @brief Widget::delSuspendQue
 * @param del
 * 删除挂起队列中的指定指针，并刷新挂起队列表格
 */
void Widget::delSuspendQue(SProcess *del)
{
    for(int i=0;i<num_in_suspend;i++){
        if(sustasks[i]->getPCB()->getPID()==del->getPCB()->getPID()){//找到指定指针
            for(int j=i;j<num_in_suspend;j++)
            {
                sustasks[j] = sustasks[j+1];//删除指针并迭代填补空位
            }
            num_in_suspend--;
            refreshSuspendQue();
            return;
        }
    }
}

/**
 * @brief Widget::refreshSuspendQue
 * 刷新挂起队列列表
 */
void Widget::refreshSuspendQue()
{
    ui->suspendque->clearContents();
    ui->suspendque->setRowCount(0);
    for(int i=0;i<num_in_suspend;i++)
    {
        int RowCont = ui->suspendque->rowCount();
        ui->suspendque->insertRow(RowCont);//增加一行
        ui->suspendque->setItem(RowCont,0,new QTableWidgetItem(sustasks[i]->getPCB()->getPID()));
        ui->suspendque->setItem(RowCont,1,new QTableWidgetItem(QString::number(sustasks[i]->getPCB()->getPriority())) );
        ui->suspendque->setItem(RowCont,2,new QTableWidgetItem(QString::number(sustasks[i]->getPCB()->getRuntime())) );
    }
}

/**
 * @brief Widget::searchProcsInSuspdQue
 * @param PID
 * @param pri
 * @param tim
 * @return 返回在挂起队列中查找的结果指针
 */
SProcess *Widget::searchProcsInSuspdQue(QString PID, int pri, int tim)
{
    for(int i=0;i<num_in_suspend;i++){
        if(sustasks[i]->getPCB()->getPID()==PID && sustasks[i]->getPCB()->getPriority()==pri
                &&sustasks[i]->getPCB()->getRuntime()==tim)
        {
            return sustasks[i];
        }
    }
    qDebug()<<"not find in searchProcsInSuspdQue of widget";
    return nullptr;
}

/**
 * @brief Widget::crushRunningProcs
 * 抢占监听
 */
void Widget::crushRunningProcs()
{
    if(readyque->getMax_Procs()>0 && moveProcs!=nullptr && !ifcpuWaitover){
        if(readyque->getMaxPrioProcs()->getPCB()->getPriority() > moveProcs->getPCB()->getPriority()){
            moveProcs->setPstatus(READY);
            moveProcs->getPCB()->setStatus(READY);
            curRunTime = 0;//清空运行时间记录器
            canDownMoveWithSlice = true;
            ifcpuWaitover = true;
            cpuWait = false;
            ui->status->setText("调度中");
            ui->status_2->setVisible(true);
//            ui->status_2->setVisible(true);
//            qDebug()<<readyque->getMaxPrioProcs()->getPCB()->getPriority();
        }
        else {
            return;
        }
    }
    else {
        return;
    }
}

void Widget::timerEvent(QTimerEvent *eve)
{
    if(eve->timerId()==timerup){
        upBtn_Move_1();
    }
    else if (eve->timerId()==timercpuwait) {
        cpuReceivedtoWait();
    }
    else if(eve->timerId()==timerdown) {
        downBtn_Move_1();
    }
    else if (eve->timerId()==timercpudeltime) {
        delMoveProcsTime();
    }
    else if (eve->timerId()==timerstartall) {
        CPUscheduling();
    }
    else if (eve->timerId()==timerMonitNum) {
        workScheduling();
    }
    else if (eve->timerId()==timerdownwithslice) {
        downBtn_Move_1_withslice();
    }
    else if (eve->timerId()==timercrush) {
        crushRunningProcs();
    }
}


void Widget::on_Start_clicked()
{
    initTimers();
    ifCPUschedule = true;
}

void Widget::on_Terminate_clicked()
{
    ifCPUschedule = false;
    ifCPUscheduleIsRunning = false;
    stopTimers();
}

/*
 * 随机添加10个作业进入后备队列
 */
void Widget::on_Random_clicked()
{
    for(int i=0;i<10;i++){
        QSqlDatabase db = getDB();
        bool ok = db.open();

        if(ok){
            QString pname = QString::number(random(10,99));
            QString intprio = QString::number(random(10,999));
            QString intrunt = QString::number(random(10,20));
            QString sql = QString("INSERT INTO backupque(pname, pstatus, priority, runtime)"
                                  " VALUES('%1', 0, %2, %3)").arg(pname).arg(intprio).arg(intrunt);
            QSqlQuery insert(db);
            bool success = insert.exec(sql);
            if(success)
            {
                qDebug()<< "插入成功";
            }
            else
            {
                QMessageBox::information(nullptr, "database connect error", "数据库连接失败");
                return;
            }
        }
        else {
           qDebug()<< "Database open failed in on_Random_clicked()";
           return;
        }
    }
}

void Widget::on_continue_2_clicked()
{
    ifCPUscheduleIsRunning = true;
    initTimers();
}

/**
 * @brief Widget::on_Suspend_clicked
 * 点击挂起，弹出可挂起队列
 */
void Widget::on_Suspend_clicked()
{
    if(ifCPUscheduleIsRunning)
        on_Terminate_clicked();
    selesuspd->init();
    selesuspd->initSuspend(readyque->getMax_Procs(), readyque->getReadyQue(), moveProcs);
    selesuspd->initTable();
    selesuspd->show();//初始化可挂起队列表格并显示之
}

/**
 * @brief Widget::addtoSuspendQue
 * @param receive
 * 选择加入挂起的进程之后确定处理逻辑的槽函数
 */
void Widget::addtoSuspendQue(SProcess *receive)
{
//    qDebug()<<receive->getPCB()->getPID();
    if(receive->getPCB()->getStatus()==READY)//如果属于ready队列，直接从ready中删除然后进挂起队列，各种刷新
    {
        readyque->delSpecifiedProcsInReady(receive);//删除receive指针（在ready中）
    }
    if(receive->getPCB()->getStatus()==RUNNING)
    {
        curRunTime = 0;
        canUpMove = false;
        cpuWait = false;
        canDownMove = false;
        ifcpuWaitover = true;
        ifCPUschedule = true;//设置状态值使得从cpu调度开头开始
        moveProcs = nullptr;//置空正在移动的进程
        initMoveBtn_1();//刷新移动按钮位置
    }
    selesuspd->init();
    selesuspd->initSuspend(readyque->getMax_Procs(), readyque->getReadyQue(), moveProcs);
    selesuspd->initTable();//刷新可挂起队列表格

    receive->getPCB()->setStatus(SUSPEND);
    addSuspendQue(receive);
    refreshSuspendQue();
}

void Widget::on_DeSuspend_clicked()
{
    if(!ui->suspendque->selectedItems().empty()){
        int row = ui->suspendque->currentRow();
        QString pid = ui->suspendque->item(row,0)->text();
        int pri = ui->suspendque->item(row,1)->text().toInt();
        int tim = ui->suspendque->item(row,2)->text().toInt();
        SProcess *selected = searchProcsInSuspdQue(pid, pri, tim);

        if(selected!=nullptr)
        {
            delSuspendQue(selected);//删除挂起队列中的该指针
            selected->getPCB()->setStatus(READY);
            addIntoReady(selected);//添加进ready队列
        }
    }
    else {
        QMessageBox::warning(nullptr, "error", "请选择");
    }
}
