#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("Images/mainwindowIcon.png"));

    UsingWays *u=new UsingWays();
    u->show();

    addnewprocessdialog= new AddNewProcessDialog();//设置添加进程的指针
    PcbController= new AllPcbController();//设置PCB管理指针
    addnewprocessdialog->setPcbControllerPointer(PcbController);//将pcbcontroller设置到dialog里
    menuBar()->setVisible(false);//隐藏菜单栏
    ui->mainToolBar->setVisible(false);//隐藏工具栏
    setWindowTitle("CPU调度模拟");
    setFixedSize(1860,984);
    createAllConnect();
    setFirstButton();//初始化所有按钮

    //waiting队列
    m_tableModel =new  QStandardItemModel();
    ui->waiting_tableView->setModel(m_tableModel);

    //terminate队列
    t_tableModel =new QStandardItemModel();
    ui->terminate_tableView->setModel(t_tableModel);

    //hanging队列
    h_tableModel =new QStandardItemModel();
    ui->hanging_tableView->setModel(h_tableModel);

    //主存TableView
    memory_tableModel = new QStandardItemModel();
    ui->memory_tableView->setModel(memory_tableModel);

    //仪表盘指针
    dashboard = new Dashboard(this);
    dashboard->updateValue(10.0*100/64);
    dashboard->move(1400,750);
    showTableViewHead();

    m_animation = new QPropertyAnimation();
    m_animation->setTargetObject(&(FirstButton));    //设置使用动画的控件
    m_animation->setEasingCurve(QEasingCurve::Linear); //设置动画效果

    //自动运行
    tim=new QTimer(this);
    connect(tim,SIGNAL(timeout()), this,SLOT(on_Next_pushButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reWaiting()
{
    //此函数的作用是显示Waiting表格中的内容
    /*初始化waiting队列*/
    qDebug()<<"调用reWaiting第一次";
    int row=0;//记录行数
    int lie=0;//记录列数
    m_tableModel->clear();//非常非常非常重要！！！！
    m_tableModel->setHorizontalHeaderItem(0, new QStandardItem("进程名"));
    m_tableModel->setHorizontalHeaderItem(1, new QStandardItem("运行时间"));
    m_tableModel->setHorizontalHeaderItem(2, new QStandardItem("优先权"));
    m_tableModel->setHorizontalHeaderItem(3, new QStandardItem("起址"));
    m_tableModel->setHorizontalHeaderItem(4,new QStandardItem("大小"));
    //setColumnWidth设置每一列的宽度
    this->ui->waiting_tableView->setColumnWidth(0, 140); //设置列的宽度
    this->ui->waiting_tableView->setColumnWidth(1, 140);
    this->ui->waiting_tableView->setColumnWidth(2, 140);
    this->ui->waiting_tableView->setColumnWidth(3, 140);
    this->ui->waiting_tableView->setColumnWidth(4, 140);

    /*设置行字段名*/
    m_tableModel->setRowCount(100);
    for (int i=0;i<100;i++) {
        QString qi=QString::number(i);
        m_tableModel->setHeaderData(i,Qt::Vertical, qi);
    }
    for (int i=0;i<PcbController->getWaitingList().length();i++) {
        QString name=PcbController->getWaitingList().at(i).getPcbName();
        int Time=PcbController->getWaitingList().at(i).getPcbTime();
        int pri=PcbController->getWaitingList().at(i).getPriority();
        int startpos=PcbController->getWaitingList().at(i).getPcbStartPos();
        int size=PcbController->getWaitingList().at(i).getPcbSize();
        //setItem函数的第一个参数表示行号，第二个表示列号，第三个为要显示的数据
        m_tableModel->setItem(row,lie,new QStandardItem(name));
        lie++;
        m_tableModel->setItem(row,lie,new QStandardItem(QString::number(Time)));
        lie++;
        m_tableModel->setItem(row,lie,new QStandardItem(QString::number(pri)));
        lie++;
        m_tableModel->setItem(row,lie,new QStandardItem(QString::number(startpos)));
        lie++;
        m_tableModel->setItem(row,lie,new QStandardItem(QString::number(size)));
        lie++;
        row++;
        lie=0;
    }
//    QVector<PCB>::iterator iter;
//    //油饼？有时候可以有时候不行，煞笔
//    for (iter=PcbController->getWaitingList().begin();iter!=PcbController->getWaitingList().end();iter++){
//        QString n=PcbController->getWaitingList().begin()->getPcbName();
//        qDebug()<<n;
//        QString name=iter->getPcbName();
//        qDebug()<<name;
//        int Time=iter->getPcbTime();
//        int pri=iter->getPriority();
//        //setItem函数的第一个参数表示行号，第二个表示列号，第三个为要显示的数据
//        m_tableModel->setItem(row,lie,new QStandardItem(name));
//        lie++;
//        m_tableModel->setItem(row,lie,new QStandardItem(QString::number(Time)));
//        lie++;
//        m_tableModel->setItem(row,lie,new QStandardItem(QString::number(pri)));
//        row++;
//        lie=0;
//    }
    qDebug()<<"调用reWaiting第二次";
}

void MainWindow::showTableViewHead()
{
      reWaiting();//waiting
      showTerminate();//terminate
      showHanging();//hanging
      showMemory();//memory
}

void MainWindow::createAllConnect()
{
    connect(addnewprocessdialog->getUi()->SurepushButton,SIGNAL(clicked()),this,SLOT(reWaiting()));

}
void MainWindow::setFirstButton()
{
    QFont font;
    //设置文字字体
    font.setFamily("华文行楷");
    //设置文字大小为50像素
    font.setPixelSize(15);
    FirstButton.setParent(this);
    FirstButton.setGeometry(20,650,489,28);
    FirstButton.setText(tr("                    P1                            6                            4               "));
    FirstButton.setFont(font);
    FirstButton.setVisible(false);
    ui->pushButton_4->setCheckable(true);
    ui->pushButton_3->setCheckable(true);
    ui->pushButton_2->setCheckable(true);
    ui->pushButton->setCheckable(true);
    FirstButton.setCheckable(true);
    ui->pushButton_4->setDefault(false);
    ui->pushButton_3->setDefault(false);
    ui->pushButton_2->setDefault(false);
    ui->pushButton->setDefault(false);
    FirstButton.setDefault(false);
}


void MainWindow::showReadyList()
{
    //谁有内容谁显示
    ui->pushButton->setText(tr("                    空                            空                            空               "));
    ui->pushButton_2->setText(tr("                    空                            空                            空               "));
    ui->pushButton_3->setText(tr("                    空                            空                            空               "));
    ui->pushButton_4->setText(tr("                    空                            空                            空               "));
    int len=PcbController->getReadyList().length();
    if (len==0){
        return;
    }
    QString name=PcbController->getReadyList()[0].getPcbName();
    QString time=QString::number(PcbController->getReadyList()[0].getPcbTime());
    QString pri=QString::number(PcbController->getReadyList()[0].getPriority());
    qDebug()<<name+"                            "+time+"                            "+pri;
    ui->pushButton->setText(name+"                            "+time+"                            "+pri);
    ui->pushButton->setVisible(true);
    if (--len==0) return;
    qDebug()<<"已添加到pushButton";
    name=PcbController->getReadyList()[1].getPcbName();
    time=QString::number(PcbController->getReadyList()[1].getPcbTime());
    pri=QString::number(PcbController->getReadyList()[1].getPriority());
    qDebug()<<name+"                            "+time+"                            "+pri;
    ui->pushButton_2->setText(name+"                            "+time+"                            "+pri);
    ui->pushButton_2->setVisible(true);
    if (--len==0) return;
    qDebug()<<"已添加到pushButton2";
    name=PcbController->getReadyList()[2].getPcbName();
    time=QString::number(PcbController->getReadyList()[2].getPcbTime());
    pri=QString::number(PcbController->getReadyList()[2].getPriority());
    qDebug()<<name+"                            "+time+"                            "+pri;
    ui->pushButton_3->setText(name+"                            "+time+"                            "+pri);
    ui->pushButton_3->setVisible(true);
    if (--len==0) return;
    qDebug()<<"已添加到pushButton3";
    name=PcbController->getReadyList()[3].getPcbName();
    time=QString::number(PcbController->getReadyList()[3].getPcbTime());
    pri=QString::number(PcbController->getReadyList()[3].getPriority());
    qDebug()<<name+"                            "+time+"                            "+pri;
    ui->pushButton_4->setText(name+"                            "+time+"                            "+pri);
    ui->pushButton_4->setVisible(true);
    if (--len==0) return;
    qDebug()<<"已添加到pushButton4";
    qDebug()<<"readyList已显示";
}

void MainWindow::showTerminate()
{
    //此函数的作用是显示Terminate表格中的内容
    /*初始化Terminate队列*/
    int row=0;//记录行数
    int lie=0;//记录列数
    t_tableModel->clear();//非常非常非常重要！！！！
    t_tableModel->setHorizontalHeaderItem(0, new QStandardItem("进程名"));
    t_tableModel->setHorizontalHeaderItem(1, new QStandardItem("运行时间"));
    t_tableModel->setHorizontalHeaderItem(2, new QStandardItem("优先权"));
    t_tableModel->setHorizontalHeaderItem(3, new QStandardItem("起址"));
    t_tableModel->setHorizontalHeaderItem(4,new QStandardItem("大小"));
    //setColumnWidth设置每一列的宽度
    this->ui->terminate_tableView->setColumnWidth(0, 140); //设置列的宽度
    this->ui->terminate_tableView->setColumnWidth(1, 140);
    this->ui->terminate_tableView->setColumnWidth(2, 140);
    this->ui->terminate_tableView->setColumnWidth(3, 140);
    this->ui->terminate_tableView->setColumnWidth(4, 140);
    /*设置行字段名*/
    t_tableModel->setRowCount(100);
    for (int i=0;i<100;i++) {
        QString qi=QString::number(i);
        t_tableModel->setHeaderData(i,Qt::Vertical, qi);
    }
    for (int i=0;i<PcbController->getTerminateList().length();i++) {
        QString name=PcbController->getTerminateList().at(i).getPcbName();
        int Time=PcbController->getTerminateList().at(i).getPcbTime();
        int pri=PcbController->getTerminateList().at(i).getPriority();
        int startpos=PcbController->getTerminateList().at(i).getPcbStartPos();
        int size=PcbController->getTerminateList().at(i).getPcbSize();
        //setItem函数的第一个参数表示行号，第二个表示列号，第三个为要显示的数据
        t_tableModel->setItem(row,lie,new QStandardItem(name));
        lie++;
        t_tableModel->setItem(row,lie,new QStandardItem(QString::number(Time)));
        lie++;
        t_tableModel->setItem(row,lie,new QStandardItem(QString::number(pri)));
        lie++;
        t_tableModel->setItem(row,lie,new QStandardItem(QString::number(startpos)));
        lie++;
        t_tableModel->setItem(row,lie,new QStandardItem(QString::number(size)));
        lie++;
        row++;
        lie=0;
    }
}

void MainWindow::showHanging()
{
    //此函数的作用是显示Terminate表格中的内容
    /*初始化Terminate队列*/
    int row=0;//记录行数
    int lie=0;//记录列数
    h_tableModel->clear();//非常非常非常重要！！！！
    h_tableModel->setHorizontalHeaderItem(0, new QStandardItem("进程名"));
    h_tableModel->setHorizontalHeaderItem(1, new QStandardItem("运行时间"));
    h_tableModel->setHorizontalHeaderItem(2, new QStandardItem("优先权"));
    h_tableModel->setHorizontalHeaderItem(3, new QStandardItem("起址"));
    h_tableModel->setHorizontalHeaderItem(4,new QStandardItem("大小"));
    //setColumnWidth设置每一列的宽度
    this->ui->hanging_tableView->setColumnWidth(0, 140); //设置列的宽度
    this->ui->hanging_tableView->setColumnWidth(1, 140);
    this->ui->hanging_tableView->setColumnWidth(2, 140);
    this->ui->hanging_tableView->setColumnWidth(3, 140);
    this->ui->hanging_tableView->setColumnWidth(4, 140);
    /*设置行字段名*/
    h_tableModel->setRowCount(100);
    for (int i=0;i<100;i++) {
        QString qi=QString::number(i);
        h_tableModel->setHeaderData(i,Qt::Vertical, qi);
    }
    for (int i=0;i<PcbController->getHangingList().length();i++) {
        QString name=PcbController->getHangingList().at(i).getPcbName();
        int Time=PcbController->getHangingList().at(i).getPcbTime();
        int pri=PcbController->getHangingList().at(i).getPriority();
        int startpos=PcbController->getHangingList().at(i).getPcbStartPos();
        int size=PcbController->getHangingList().at(i).getPcbSize();
        //setItem函数的第一个参数表示行号，第二个表示列号，第三个为要显示的数据
        h_tableModel->setItem(row,lie,new QStandardItem(name));
        lie++;
        h_tableModel->setItem(row,lie,new QStandardItem(QString::number(Time)));
        lie++;
        h_tableModel->setItem(row,lie,new QStandardItem(QString::number(pri)));
        lie++;
        h_tableModel->setItem(row,lie,new QStandardItem(QString::number(startpos)));
        lie++;
        h_tableModel->setItem(row,lie,new QStandardItem(QString::number(size)));
        lie++;
        row++;
        lie=0;
    }
}

void MainWindow::showMemory()
{
    //此函数的作用是显示Memory表格中的内容
    /*初始化Memory队列*/
    int row=0;//记录行数
    int lie=0;//记录列数
    memory_tableModel->clear();//非常非常非常重要！！！！
    memory_tableModel->setHorizontalHeaderItem(0, new QStandardItem("进程名"));
    memory_tableModel->setHorizontalHeaderItem(1, new QStandardItem("起址"));
    memory_tableModel->setHorizontalHeaderItem(2, new QStandardItem("长度"));
    //setColumnWidth设置每一列的宽度
    this->ui->memory_tableView->setColumnWidth(0, 140); //设置列的宽度
    this->ui->memory_tableView->setColumnWidth(1, 140);
    this->ui->memory_tableView->setColumnWidth(2, 140);
    /*设置行字段名*/
    memory_tableModel->setRowCount(64);
    for (int i=0;i<64;i++) {
        QString qi=QString::number(i);
        memory_tableModel->setHeaderData(i,Qt::Vertical, qi);
    }
    double sum=0;
    for (int i=0;i<PcbController->getMemoryList().length();i++){
        //先粗略的显示
        row=PcbController->getMemoryList()[i].getStartpos();
        QString pcbName = PcbController->getMemoryList()[i].getPid();
        int pcbStart = PcbController->getMemoryList()[i].getStartpos();
        int pcbLength = PcbController->getMemoryList()[i].getLength();
        sum+=pcbLength;
        memory_tableModel->setItem(row,lie,new QStandardItem(pcbName));
        memory_tableModel->item(row,lie)->setBackground(QColor("#D2B48C"));
        lie++;
        memory_tableModel->setItem(row,lie,new QStandardItem(QString::number(pcbStart)));
        memory_tableModel->item(row,lie)->setBackground(QColor("#D2B48C"));
        lie++;
        memory_tableModel->setItem(row,lie,new QStandardItem(QString::number(pcbLength)));
        memory_tableModel->item(row,lie)->setBackground(QColor("#D2B48C"));
        lie++;
        lie=0;
        if (i!=0){
            for(int j=0;j<pcbLength-1;j++){
                int row2  = row+1+j;
                memory_tableModel->setItem(row2,lie,new QStandardItem(""));
                memory_tableModel->item(row2,lie)->setBackground(QColor("#FFE4C4"));
                lie++;
                memory_tableModel->setItem(row2,lie,new QStandardItem(""));
                memory_tableModel->item(row2,lie)->setBackground(QColor("#FFE4C4"));
                lie++;
                memory_tableModel->setItem(row2,lie,new QStandardItem(""));
                memory_tableModel->item(row2,lie)->setBackground(QColor("#FFE4C4"));
                lie++;
                lie=0;
            }
        }else{
            //如果是操作系统进程
            for(int j=0;j<pcbLength-1;j++){
                int row2  = row+1+j;
                memory_tableModel->setItem(row2,lie,new QStandardItem(""));
                memory_tableModel->item(row2,lie)->setBackground(QColor("#DAA520"));
                lie++;
                memory_tableModel->setItem(row2,lie,new QStandardItem(""));
                memory_tableModel->item(row2,lie)->setBackground(QColor("#DAA520"));
                lie++;
                memory_tableModel->setItem(row2,lie,new QStandardItem(""));
                memory_tableModel->item(row2,lie)->setBackground(QColor("#DAA520"));
                lie++;
                lie=0;
            }
        }

    }
    dashboard->updateValue(sum*100/64);
}


void MainWindow::GoToRunning()
{
    if (PcbController->getRunningList().length()==0){
        FirstButton.setVisible(false);
        return;
    }
    FirstButton.setGeometry(20,650,489,28);
    FirstButton.setVisible(true);
    QString name=PcbController->getRunningList()[0].getPcbName();
    QString time=QString::number(PcbController->getRunningList()[0].getPcbTime());
    QString pri=QString::number(PcbController->getRunningList()[0].getPriority());
    FirstButton.setText(name+"                            "+time+"                            "+pri);

    m_animation->setPropertyName("geometry");   //指定动画属性名
    m_animation->setDuration(1000);
    m_animation->setStartValue(QRect(20,650,489,28));
    m_animation->setEndValue(QRect(790,745,489,28));
    m_animation->start();
}




void MainWindow::on_AddProcess_pushButton_clicked()
{
    addnewprocessdialog->show();
}

void MainWindow::on_clearWaiting_pushButton_clicked()
{
    int cnt=0;
    QVector<PCB> w1=PcbController->getWaitingList();
    for (int i=0;i<w1.length();i++) {
        QString name=w1.at(i).getPcbName();
        PcbController->deleteWaiting(name);
        cnt++;
    }
    reWaiting();//重新显示Waiting队列
}

void MainWindow::on_pushButton_4_clicked(bool checked)
{
    if(checked == true)
        {
            qDebug()<<"111";//选中
            ui->pushButton_4->setDefault(false);
        }
        else
        {
            qDebug()<<"222";//取消选中
            ui->pushButton_4->setDefault(true);
        }
}

void MainWindow::on_pushButton_3_clicked(bool checked)
{
    if(checked == true)
        {
            qDebug()<<"111";//选中
            ui->pushButton_3->setDefault(false);
        }
        else
        {
            qDebug()<<"222";//取消选中
            ui->pushButton_3->setDefault(true);
        }
}

void MainWindow::on_pushButton_2_clicked(bool checked)
{
    if(checked == true)
        {
            qDebug()<<"111";//选中
            ui->pushButton_2->setDefault(false);
        }
        else
        {
            qDebug()<<"222";//取消选中
            ui->pushButton_2->setDefault(true);
        }
}

void MainWindow::on_pushButton_clicked(bool checked)
{
    if(checked == true)
        {
            qDebug()<<"111";//选中
            ui->pushButton->setDefault(false);
        }
        else
        {
            qDebug()<<"222";//取消选中
            ui->pushButton->setDefault(true);
        }
}

void MainWindow::on_Start_pushButton_clicked()
{
    if (isCanStart){
        if (QString::compare(ui->comboBox_2->currentText(),"自动运行")==0){
            tim->start(1000);
            Auto=true;
        }else{
            tim->stop();
            Auto=false;
        }
        if (QString::compare(ui->comboBox->currentText(),"优先级调度")==0){
            PriIsChange=true;
            if (PcbController->getWaitingList().length()<4){
                //Waiting长度小于4，则调用waiting长度的PCB
                if (QString::compare(ui->comboBox_3->currentText(),"首次适应")==0){
                    PcbController->transferWaitingToReady(PcbController->getWaitingList().length());
                    FIFOFit=true;
                }else if (QString::compare(ui->comboBox_3->currentText(),"最优适应")==0){
                     PcbController->transferWaitingToReadyBest(PcbController->getWaitingList().length());
                     BestFit=true;
                }else if (QString::compare(ui->comboBox_3->currentText(),"最差适应")==0){
                    PcbController->transferWaitingToReadyWorst(PcbController->getWaitingList().length());
                    WorstFit=true;
                }
            }else{
                if (QString::compare(ui->comboBox_3->currentText(),"首次适应")==0){
                    PcbController->transferWaitingToReady(4);
                    FIFOFit=true;
                }else if (QString::compare(ui->comboBox_3->currentText(),"最优适应")==0){
                     PcbController->transferWaitingToReadyBest(4);
                     BestFit=true;
                }else if (QString::compare(ui->comboBox_3->currentText(),"最差适应")==0){
                    PcbController->transferWaitingToReadyWorst(4);
                    WorstFit=true;
                }
            }
            PcbController->SortReadyList();//排序readyList
            showMemory();//显示主存信息
            reWaiting();
            showReadyList();
            PcbController->ReadyChangeToRunning();
            if (PcbController->getWaitingList().length()>0){
                if (FIFOFit){
                   PcbController->transferWaitingToReady(1);//来一个，来的了就来，来不了就不来
                }else if (BestFit){
                   PcbController->transferWaitingToReadyBest(1);//来一个，来的了就来，来不了就不来
                }else if (WorstFit){
                   PcbController->transferWaitingToReadyWorst(1);//来一个，来的了就来，来不了就不来
                }
                PcbController->SortReadyList();//排序readyList
                reWaiting();//Waiting重新加载
                showMemory();//显示主存信息
                showReadyList();//Ready重新显示
            }
            if (!(PcbController->ReadyChangeToRunning())){
                //发生抢占
                qDebug()<<"发生抢占";
                //FirstButton.setVisible(false);
                showReadyList();
                GoToRunning();
            }else{
                showReadyList();
                GoToRunning();
            }
            isCanStart=false;
        }else{
            //时间片轮转,不需要排序了
            //先waiting到ready
            TimerIsChange=true;
            if (PcbController->getWaitingList().length()<4){
                //Waiting长度小于4，则调用waiting长度的PCB
                if (QString::compare(ui->comboBox_3->currentText(),"首次适应")==0){
                    PcbController->transferWaitingToReady(PcbController->getWaitingList().length());
                    FIFOFit=true;
                }else if (QString::compare(ui->comboBox_3->currentText(),"最优适应")==0){
                     PcbController->transferWaitingToReadyBest(PcbController->getWaitingList().length());
                     BestFit=true;
                }else if (QString::compare(ui->comboBox_3->currentText(),"最差适应")==0){
                    PcbController->transferWaitingToReadyWorst(PcbController->getWaitingList().length());
                    WorstFit=true;
                }
            }else{
                if (QString::compare(ui->comboBox_3->currentText(),"首次适应")==0){
                    PcbController->transferWaitingToReady(4);
                    FIFOFit=true;
                }else if (QString::compare(ui->comboBox_3->currentText(),"最优适应")==0){
                     PcbController->transferWaitingToReadyBest(4);
                     BestFit=true;
                }else if (QString::compare(ui->comboBox_3->currentText(),"最差适应")==0){
                    PcbController->transferWaitingToReadyWorst(4);
                    WorstFit=true;
                }
            }
            showMemory();
            reWaiting();
            showReadyList();
            PcbController->ReadyChangeToRunningTimer();
            if (PcbController->getWaitingList().length()>0){
                if (FIFOFit){
                   PcbController->transferWaitingToReady(1);//来一个，来的了就来，来不了就不来
                }else if (BestFit){
                   PcbController->transferWaitingToReadyBest(1);//来一个，来的了就来，来不了就不来
                }else if (WorstFit){
                   PcbController->transferWaitingToReadyWorst(1);//来一个，来的了就来，来不了就不来
                }
                //这里的逻辑都得改，因为不一定能成功进ready，但是我现在有点饿，不想改了，回去再改 11.16.9:24
                showMemory();
                reWaiting();//Waiting重新加载
                showReadyList();//Ready重新显示
            }
            if (!(PcbController->ReadyChangeToRunningTimer())){
                //发生抢占
                qDebug()<<"发生时间片抢占";
                //FirstButton.setVisible(false);
                showReadyList();
                GoToRunning();
            }else{
                showReadyList();
                GoToRunning();
            }
            isCanStart=false;
        }
    }else{
        QMessageBox box;
        box.setWindowTitle(tr("已经开始，请勿重复点击"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请点击下一步或者重新开始"));
        box.addButton(tr("确定"),QMessageBox::AcceptRole);
        if(box.exec()==QMessageBox::Accepted)
        {
            box.close();
        }
    }

}

void MainWindow::on_Next_pushButton_clicked()
{
    if (isCanStart){
        //还没开始
        QMessageBox::warning(this,tr("未开始"),"请先点击开始运行");
        return;
    }
    ui->label_5->setText("");
    if (QString::compare(ui->comboBox->currentText(),"优先级调度")==0){
        if (TimerIsChange){
            //说明刚刚用的是时间片调度算法
            QMessageBox::information(this,tr("更改算法"),"已更改为优先级调度算法");
            TimerIsChange=false;
            PriIsChange=true;
        }
        if (QString::compare(ui->comboBox_3->currentText(),"首次适应")==0 && (BestFit||WorstFit)){
            QMessageBox::information(this,tr("更改内存分配算法"),"已更改为首次适应算法");
            FIFOFit=true;
            BestFit=false;
            WorstFit=false;
        }
        if (QString::compare(ui->comboBox_3->currentText(),"最优适应")==0 && (FIFOFit||WorstFit)){
            QMessageBox::information(this,tr("更改内存分配算法"),"已更改为最优适应算法");
            FIFOFit=false;
            BestFit=true;
            WorstFit=false;
        }
        if (QString::compare(ui->comboBox_3->currentText(),"最差适应")==0 && (FIFOFit||BestFit)){
            QMessageBox::information(this,tr("更改内存分配算法"),"已更改为最差适应算法");
            FIFOFit=false;
            BestFit=false;
            WorstFit=true;
        }
        bool f=PcbController->aging();
        if (!(f)&&PcbController->getRunningList().length()!=0){
            //要去终止了
            QString name=PcbController->getRunningList()[0].getPcbName();
            QString time=QString::number(PcbController->getRunningList()[0].getPcbTime());
            QString pri=QString::number(PcbController->getRunningList()[0].getPriority());
            FirstButton.setText(name+"                            "+time+"                            "+pri);
            PcbController->RunningToTerminate();
            showTerminate();
            PcbController->ReadyChangeToRunning();//running从0到1
            //ui->pushButton->setVisible(false);
            int readyLength = PcbController->getReadyList().length();
            if (PcbController->getWaitingList().length()>=(4-readyLength)){
                if (FIFOFit){
                   PcbController->transferWaitingToReady(4-readyLength);//来一个，来的了就来，来不了就不来
                }else if (BestFit){
                   PcbController->transferWaitingToReadyBest(4-readyLength);//来一个，来的了就来，来不了就不来
                }else if (WorstFit){
                   PcbController->transferWaitingToReadyWorst(4-readyLength);//来一个，来的了就来，来不了就不来
                }
                PcbController->SortReadyList();//排序readyList
                reWaiting();//Waiting重新加载
                showMemory();
                showReadyList();//Ready重新显示
                GoToRunning();
            }else{
                if (PcbController->getWaitingList().length()>0){
//                    if (QString::compare(ui->comboBox_3->currentText(),"首次适应")==0 && (BestFit||WorstFit)){
//                        QMessageBox::information(this,tr("更改内存分配算法"),"已更改为首次适应算法");
//                        FIFOFit=true;
//                        BestFit=false;
//                        WorstFit=false;
//                    }
//                    if (QString::compare(ui->comboBox_3->currentText(),"最优适应")==0 && (FIFOFit||WorstFit)){
//                        QMessageBox::information(this,tr("更改内存分配算法"),"已更改为最优适应算法");
//                        FIFOFit=false;
//                        BestFit=true;
//                        WorstFit=false;
//                    }
//                    if (QString::compare(ui->comboBox_3->currentText(),"最差适应")==0 && (FIFOFit||BestFit)){
//                        QMessageBox::information(this,tr("更改内存分配算法"),"已更改为最差适应算法");
//                        FIFOFit=false;
//                        BestFit=false;
//                        WorstFit=true;
//                    }
                    if (FIFOFit){
                       PcbController->transferWaitingToReady(PcbController->getWaitingList().length());//来一个，来的了就来，来不了就不来
                    }else if (BestFit){
                       PcbController->transferWaitingToReadyBest(PcbController->getWaitingList().length());//来一个，来的了就来，来不了就不来
                    }else if (WorstFit){
                       PcbController->transferWaitingToReadyWorst(PcbController->getWaitingList().length());//来一个，来的了就来，来不了就不来
                    }
                }
                reWaiting();//Waiting重新加载
                showMemory();
                showReadyList();//Ready重新显示
                GoToRunning();
            }
            if (PcbController->getReadyList().length()==0){
                return;
            }
            if (!(PcbController->ReadyChangeToRunning())){
                //发生抢占
                qDebug()<<"发生抢占";
                //FirstButton.setVisible(false);
                ui->label_5->setText("发生抢占！");
                showReadyList();
                GoToRunning();
            }
        }else if(!(f)&&PcbController->getRunningList().length()==0){
            //runninglist也没有，可以不用管
            if (PcbController->getWaitingList().length()>0){
                if (PcbController->getWaitingList().length()<4){
                    //Waiting长度小于4，则调用waiting长度的PCB
                    PcbController->transferWaitingToReady(PcbController->getWaitingList().length());
                }else{
                    PcbController->transferWaitingToReady(4);//转运四个waitingPcb
                }
                PcbController->SortReadyList();//排序readyList
                showMemory();
                reWaiting();
                showReadyList();
                PcbController->ReadyChangeToRunning();
                if (PcbController->getWaitingList().length()>0){
                    PcbController->transferWaitingToReady(1);//来一个
                    PcbController->SortReadyList();//排序readyList
                    showMemory();
                    reWaiting();//Waiting重新加载
                    showReadyList();//Ready重新显示
                }
                if (!(PcbController->ReadyChangeToRunning())){
                    //发生抢占
                    qDebug()<<"发生抢占";
                    //FirstButton.setVisible(false);
                    ui->label_5->setText("发生抢占！");
                    showReadyList();
                    GoToRunning();
                }else{
                    showReadyList();
                    GoToRunning();
                }
            }else{
                if (!Auto){
                    QMessageBox box;
                    box.setWindowTitle(tr("Running无内容"));
                    box.setIcon(QMessageBox::Warning);
                    box.setText(tr("已无进程可以调度，请重新添加进程"));
                    box.addButton(tr("确定"),QMessageBox::AcceptRole);
                    if(box.exec()==QMessageBox::Accepted)
                    {
                        box.close();
                    }
                }
            }
        }else{
            //正常情况
            QString name=PcbController->getRunningList()[0].getPcbName();
            QString time=QString::number(PcbController->getRunningList()[0].getPcbTime());
            QString pri=QString::number(PcbController->getRunningList()[0].getPriority());
            FirstButton.setText(name+"                            "+time+"                            "+pri);
            if (PcbController->getReadyList().length()==0){
                return;
            }
            if (!(PcbController->ReadyChangeToRunning())){
                //发生抢占
                //条件自带排序
                qDebug()<<"发生抢占";
                ui->label_5->setText("发生抢占！");
                showReadyList();
                GoToRunning();
            }
        }
    }else{
        //时间片调度算法
        if (PriIsChange){
            //说明刚刚用的是时间片调度算法
            QMessageBox::information(this,tr("更改算法"),"已更改为时间片轮转调度算法");
            TimerIsChange=true;
            PriIsChange=false;
        }
        if (QString::compare(ui->comboBox_3->currentText(),"首次适应")==0 && (BestFit||WorstFit)){
            QMessageBox::information(this,tr("更改内存分配算法"),"已更改为首次适应算法");
            FIFOFit=true;
            BestFit=false;
            WorstFit=false;
        }
        if (QString::compare(ui->comboBox_3->currentText(),"最优适应")==0 && (FIFOFit||WorstFit)){
            QMessageBox::information(this,tr("更改内存分配算法"),"已更改为最优适应算法");
            FIFOFit=false;
            BestFit=true;
            WorstFit=false;
        }
        if (QString::compare(ui->comboBox_3->currentText(),"最差适应")==0 && (FIFOFit||BestFit)){
            QMessageBox::information(this,tr("更改内存分配算法"),"已更改为最差适应算法");
            FIFOFit=false;
            BestFit=false;
            WorstFit=true;
        }
        bool f=PcbController->agingTimer();
        if (!(f)&&PcbController->getRunningList().length()!=0){
            //要去终止了
            QString name=PcbController->getRunningList()[0].getPcbName();
            QString time=QString::number(PcbController->getRunningList()[0].getPcbTime());
            QString pri=QString::number(PcbController->getRunningList()[0].getPriority());
            FirstButton.setText(name+"                            "+time+"                            "+pri);
            PcbController->RunningToTerminate();
            showTerminate();
            PcbController->ReadyChangeToRunningTimer();//running从0到1
            //ui->pushButton->setVisible(false);
            int readyLength =PcbController->getReadyList().length();
            if (PcbController->getWaitingList().length()>=(4-readyLength)){
                if (FIFOFit){
                   PcbController->transferWaitingToReady(4-readyLength);//来一个，来的了就来，来不了就不来
                }else if (BestFit){
                   PcbController->transferWaitingToReadyBest(4-readyLength);//来一个，来的了就来，来不了就不来
                }else if (WorstFit){
                   PcbController->transferWaitingToReadyWorst(4-readyLength);//来一个，来的了就来，来不了就不来
                }
                showMemory();
                reWaiting();//Waiting重新加载
                showReadyList();//Ready重新显示
                GoToRunning();
            }else{
                if (PcbController->getWaitingList().length()>0){
//                    if (QString::compare(ui->comboBox_3->currentText(),"首次适应")==0 && (BestFit||WorstFit)){
//                        QMessageBox::information(this,tr("更改内存分配算法"),"已更改为首次适应算法");
//                        FIFOFit=true;
//                        BestFit=false;
//                        WorstFit=false;
//                    }
//                    if (QString::compare(ui->comboBox_3->currentText(),"最优适应")==0 && (FIFOFit||WorstFit)){
//                        QMessageBox::information(this,tr("更改内存分配算法"),"已更改为最优适应算法");
//                        FIFOFit=false;
//                        BestFit=true;
//                        WorstFit=false;
//                    }
//                    if (QString::compare(ui->comboBox_3->currentText(),"最差适应")==0 && (FIFOFit||BestFit)){
//                        QMessageBox::information(this,tr("更改内存分配算法"),"已更改为最差适应算法");
//                        FIFOFit=false;
//                        BestFit=false;
//                        WorstFit=true;
//                    }
                    if (FIFOFit){
                       PcbController->transferWaitingToReady(PcbController->getWaitingList().length());//来一个，来的了就来，来不了就不来
                    }else if (BestFit){
                       PcbController->transferWaitingToReadyBest(PcbController->getWaitingList().length());//来一个，来的了就来，来不了就不来
                    }else if (WorstFit){
                       PcbController->transferWaitingToReadyWorst(PcbController->getWaitingList().length());//来一个，来的了就来，来不了就不来
                    }
                }
                reWaiting();//Waiting重新加载
                showMemory();
                showReadyList();//Ready重新显示
                GoToRunning();
            }
            if (PcbController->getReadyList().length()==0){
                return;
            }
            if (!(PcbController->ReadyChangeToRunningTimer())){
                //发生抢占
                qDebug()<<"发生抢占";
                ui->label_5->setText("发生抢占！");
                showReadyList();
                GoToRunning();
            }
        }else if(!(f)&&PcbController->getRunningList().length()==0){
            if (PcbController->getWaitingList().length()>0){
                if (PcbController->getWaitingList().length()<4){
                    //Waiting长度小于4，则调用waiting长度的PCB
                    PcbController->transferWaitingToReady(PcbController->getWaitingList().length());
                }else{
                    PcbController->transferWaitingToReady(4);//转运四个waitingPcb
                }
                showMemory();
                reWaiting();
                showReadyList();
                PcbController->ReadyChangeToRunningTimer();
                if (PcbController->getWaitingList().length()>0){
                    PcbController->transferWaitingToReady(1);//来一个
                    showMemory();
                    reWaiting();//Waiting重新加载
                    showReadyList();//Ready重新显示
                }
                if (!(PcbController->ReadyChangeToRunningTimer())){
                    //发生抢占
                    qDebug()<<"发生抢占";
                    ui->label_5->setText("发生抢占！");
                    showReadyList();
                    GoToRunning();
                }else{
                    showReadyList();
                    GoToRunning();
                }
            }else{
                if (!Auto){
                    QMessageBox box;
                    box.setWindowTitle(tr("Running无内容"));
                    box.setIcon(QMessageBox::Warning);
                    box.setText(tr("已无进程可以调度，请重新添加进程"));
                    box.addButton(tr("确定"),QMessageBox::AcceptRole);
                    if(box.exec()==QMessageBox::Accepted)
                    {
                        box.close();
                    }
                }
            }
        }else{
            //正常情况
            QString name=PcbController->getRunningList()[0].getPcbName();
            QString time=QString::number(PcbController->getRunningList()[0].getPcbTime());
            QString pri=QString::number(PcbController->getRunningList()[0].getPriority());
            FirstButton.setText(name+"                            "+time+"                            "+pri);
            if (PcbController->getReadyList().length()==0){
                return;
            }
            if (!(PcbController->ReadyChangeToRunningTimer())){
                //发生抢占
                qDebug()<<"发生抢占";
                ui->label_5->setText("发生抢占！");
                showReadyList();
                GoToRunning();
            }
        }
    }
}

void MainWindow::on_hanging_pushButton_clicked()
{

    if (ui->radioButton->isChecked()){
        //被选中
        qDebug()<<"按钮一被选中";
        //首先ready的进hanging(这个在controller里面实现)
        if(!(PcbController->ReadyToHanging(0))){
            QMessageBox box;
            box.setWindowTitle(tr("选中的进程不存在"));
            box.setIcon(QMessageBox::Warning);
            box.setText(tr("选中的进程为空，请重新选择或向就绪队列添加进程"));
            box.addButton(tr("确定"),QMessageBox::AcceptRole);
            if(box.exec()==QMessageBox::Accepted)
            {
                box.close();
            }
            return;
        }
        showHanging();
        //waiting会补充到ready，补充一个
        int readyLength = PcbController->getReadyList().length();
        if (PcbController->getWaitingList().length()>=(4-readyLength)){
            PcbController->transferWaitingToReady(4-readyLength);
        }
        showMemory();//逻辑要改的
        reWaiting();
        PcbController->SortReadyList();
        showReadyList();
        if (!(PcbController->ReadyChangeToRunning())){
            //发生抢占
            qDebug()<<"发生抢占";
            //FirstButton.setVisible(false);
            showReadyList();
            GoToRunning();
        }
    }else if (ui->radioButton_2->isChecked()){
        //被选中
        qDebug()<<"按钮二被选中";
        //首先ready的进hanging(这个在controller里面实现)
        if(!(PcbController->ReadyToHanging(1))){
            QMessageBox box;
            box.setWindowTitle(tr("选中的进程不存在"));
            box.setIcon(QMessageBox::Warning);
            box.setText(tr("选中的进程为空，请重新选择或向就绪队列添加进程"));
            box.addButton(tr("确定"),QMessageBox::AcceptRole);
            if(box.exec()==QMessageBox::Accepted)
            {
                box.close();
            }
            return;
        }
        showHanging();
        //waiting会补充到ready，补充一个
        int readyLength = PcbController->getReadyList().length();
        if (PcbController->getWaitingList().length()>=(4-readyLength)){
            PcbController->transferWaitingToReady(4-readyLength);
        }
        reWaiting();
        PcbController->SortReadyList();
        showMemory();//逻辑要改的
        showReadyList();
        if (!(PcbController->ReadyChangeToRunning())){
            //发生抢占
            qDebug()<<"发生抢占";
            //FirstButton.setVisible(false);
            showReadyList();
            GoToRunning();
        }
    }else if (ui->radioButton_3->isChecked()){
        //被选中
        qDebug()<<"按钮三被选中";
        //首先ready的进hanging(这个在controller里面实现)
        if(!(PcbController->ReadyToHanging(2))){
            QMessageBox box;
            box.setWindowTitle(tr("选中的进程不存在"));
            box.setIcon(QMessageBox::Warning);
            box.setText(tr("选中的进程为空，请重新选择或向就绪队列添加进程"));
            box.addButton(tr("确定"),QMessageBox::AcceptRole);
            if(box.exec()==QMessageBox::Accepted)
            {
                box.close();
            }
            return;
        }
        showHanging();
        //waiting会补充到ready，补充一个
        int readyLength = PcbController->getReadyList().length();
        if (PcbController->getWaitingList().length()>=(4-readyLength)){
            PcbController->transferWaitingToReady(4-readyLength);
        }
        reWaiting();
        PcbController->SortReadyList();
        showMemory();//逻辑要改的
        showReadyList();
        if (!(PcbController->ReadyChangeToRunning())){
            //发生抢占
            qDebug()<<"发生抢占";
            //FirstButton.setVisible(false);
            showReadyList();
            GoToRunning();
        }
    }else if (ui->radioButton_4->isChecked()){
        //被选中
        qDebug()<<"按钮四被选中";
        //首先ready的进hanging(这个在controller里面实现)
        if(!(PcbController->ReadyToHanging(3))){
            QMessageBox box;
            box.setWindowTitle(tr("选中的进程不存在"));
            box.setIcon(QMessageBox::Warning);
            box.setText(tr("选中的进程为空，请重新选择或向就绪队列添加进程"));
            box.addButton(tr("确定"),QMessageBox::AcceptRole);
            if(box.exec()==QMessageBox::Accepted)
            {
                box.close();
            }
            return;
        }
        showHanging();
        //waiting会补充到ready，补充一个
        int readyLength = PcbController->getReadyList().length();
        if (PcbController->getWaitingList().length()>=(4-readyLength)){
            PcbController->transferWaitingToReady(4-readyLength);
        }
        showMemory();//逻辑要改的
        reWaiting();
        PcbController->SortReadyList();
        showReadyList();
        if (!(PcbController->ReadyChangeToRunning())){
            //发生抢占
            qDebug()<<"发生抢占";
            //FirstButton.setVisible(false);
            showReadyList();
            GoToRunning();
        }
    }
}


void MainWindow::on_relieveHanging_pushButton_clicked()
{
    if (!(PcbController->HangingToWaiting(cell))){
        //没找到
        QMessageBox box;
        box.setWindowTitle(tr("没有找到进程"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请选择进程的名字"));
        box.addButton(tr("确定"),QMessageBox::AcceptRole);
        if(box.exec()==QMessageBox::Accepted)
        {
            box.close();
        }
    }else{
        //找到,则重新显示
        reWaiting();
        showHanging();
    }
}

void MainWindow::on_hanging_tableView_clicked(const QModelIndex &index)
{
    cell = index.data().toString();
}

void MainWindow::on_restart_pushButton_clicked()
{
    //点击重新开始会停止
    TimerIsChange=false;
    PriIsChange=false;
    tim->stop();
    Auto=false;
    isCanStart=true;
    //全部清空
    PcbController->reStart();
    showMemory();
    //重新显示waiting队列
    reWaiting();
    //重新显示ready队列
    ui->pushButton->setText(tr("                    空                            空                            空               "));
    ui->pushButton_2->setText(tr("                    空                            空                            空               "));
    ui->pushButton_3->setText(tr("                    空                            空                            空               "));
    ui->pushButton_4->setText(tr("                    空                            空                            空               "));
    FirstButton.setGeometry(20,650,489,28);
    FirstButton.setVisible(false);
    //重新显示terminate队列
    showTerminate();
    showHanging();
}

void MainWindow::on_pushButton_5_clicked()
{
    tim->stop();
    Auto=false;
    QMessageBox::information(this,tr("暂停成功"),"已暂停自动运行");
}

void MainWindow::on_pushButton_6_clicked()
{
    if (!(QString::compare(ui->comboBox_2->currentText(),"自动运行")==0)){
        //如果现在是自动运行并且不能再开始(说明已经开始了)
        QMessageBox box;
        box.setWindowTitle(tr("不可以继续"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请选择自动运行模式"));
        box.addButton(tr("确定"),QMessageBox::AcceptRole);
        if(box.exec()==QMessageBox::Accepted)
        {
            box.close();
        }
        return;
    }
    if (isCanStart){
        //能再开始(说明没开始了)
        QMessageBox box;
        box.setWindowTitle(tr("不可以继续"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请先开始运行"));
        box.addButton(tr("确定"),QMessageBox::AcceptRole);
        if(box.exec()==QMessageBox::Accepted)
        {
            box.close();
        }
        return;
    }
    tim->start(1000);
    Auto=true;
    QMessageBox::information(this,tr("继续成功"),"已继续自动运行");

}
