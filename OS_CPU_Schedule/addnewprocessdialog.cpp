#include "addnewprocessdialog.h"
#include "ui_addnewprocessdialog.h"

AddNewProcessDialog::AddNewProcessDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNewProcessDialog)
{
    ui->setupUi(this);
    setFixedSize(848,637);
    setWindowTitle("添加新进程");
    setWindowIcon(QIcon("Images/stationIcon-modified"));
    InitComboBox();
}

AddNewProcessDialog::~AddNewProcessDialog()
{
    delete ui;
}

void AddNewProcessDialog::setPcbControllerPointer(AllPcbController *p){
    PcbController=p;
}//设置PCB管理指针

void AddNewProcessDialog::InitComboBox(){
    //comboBox->addItem(a)
    //comboBox->clear()
    //comboBox->currentText()
    for(int i=1;i<=9;i++){
        //QString::number(i),int转化为QString
        ui->Pri1comboBox->addItem(QString::number(i));
    }
    for(int i=1;i<=9;i++){
        ui->Pri2comboBox->addItem(QString::number(i));
    }
    for(int i=1;i<=9;i++){
        ui->Pri3comboBox->addItem(QString::number(i));
    }
}


void AddNewProcessDialog::on_SurepushButton_clicked()
{
    //ui->SPlineEdit->text()
    //lineEdit->text().isEmpty()
    bool flag1=true;//判断输入是否有问题
    bool flag2=true;
    bool flag3=true;
    //判断是否是只有一个不为空，如果是的话，flag=false
    if (((ui->Name1lineEdit->text().isEmpty())&&!(ui->Time1lineEdit->text().isEmpty()))||(!(ui->Name1lineEdit->text().isEmpty())&&(ui->Time1lineEdit->text().isEmpty()))){
        flag1=false;
    }
    if (((ui->Name2lineEdit->text().isEmpty())&&!(ui->Time2lineEdit->text().isEmpty()))||(!(ui->Name2lineEdit->text().isEmpty())&&(ui->Time2lineEdit->text().isEmpty()))){
        flag2=false;
    }
    if (((ui->Name3lineEdit->text().isEmpty())&&!(ui->Time3lineEdit->text().isEmpty()))||(!(ui->Name3lineEdit->text().isEmpty())&&(ui->Time3lineEdit->text().isEmpty()))){
        flag3=false;
    }
    if (flag1){
        if ((ui->Name1lineEdit->text().isEmpty())&&(ui->Time1lineEdit->text().isEmpty())){
            if (!(ui->Size1lineEdit->text().isEmpty())){
                flag1=false;
            }
        }
        if (!(ui->Name1lineEdit->text().isEmpty())&&!(ui->Time1lineEdit->text().isEmpty())){
            if ((ui->Size1lineEdit->text().isEmpty())){
                flag1=false;
            }
        }
    }
    if (flag2){
        if ((ui->Name2lineEdit->text().isEmpty())&&(ui->Time2lineEdit->text().isEmpty())){
            if (!(ui->Size2lineEdit->text().isEmpty())){
                flag2=false;
            }
        }
        if (!(ui->Name2lineEdit->text().isEmpty())&&!(ui->Time2lineEdit->text().isEmpty())){
            if ((ui->Size2lineEdit->text().isEmpty())){
                flag2=false;
            }
        }
    }
    if (flag3){
        if ((ui->Name3lineEdit->text().isEmpty())&&(ui->Time3lineEdit->text().isEmpty())){
            if (!(ui->Size3lineEdit->text().isEmpty())){
                flag3=false;
            }
        }
        if (!(ui->Name3lineEdit->text().isEmpty())&&!(ui->Time3lineEdit->text().isEmpty())){
            if ((ui->Size3lineEdit->text().isEmpty())){
                flag3=false;
            }
        }
    }
    //判断运行时间是否是纯数字
    if ((!(ui->Time1lineEdit->text().contains(QRegExp("^\\d+$"))))&&(!(ui->Time1lineEdit->text().isEmpty()))){
        flag1=false;
    }
    if ((!(ui->Time2lineEdit->text().contains(QRegExp("^\\d+$"))))&&(!(ui->Time2lineEdit->text().isEmpty()))){
        flag2=false;
    }
    if ((!(ui->Time3lineEdit->text().contains(QRegExp("^\\d+$"))))&&(!(ui->Time3lineEdit->text().isEmpty()))){
        flag3=false;
    }
    //判断进程大小是否是纯数字
    if ((!(ui->Size1lineEdit->text().contains(QRegExp("^\\d+$"))))&&(!(ui->Size1lineEdit->text().isEmpty()))){
        flag1=false;
    }
    if ((!(ui->Size2lineEdit->text().contains(QRegExp("^\\d+$"))))&&(!(ui->Size2lineEdit->text().isEmpty()))){
        flag2=false;
    }
    if ((!(ui->Size3lineEdit->text().contains(QRegExp("^\\d+$"))))&&(!(ui->Size3lineEdit->text().isEmpty()))){
        flag3=false;
    }
    if ((!flag1)||(!flag2)||(!flag3)){
        QMessageBox box;
        box.setWindowTitle(tr("输入错误"));
       // box.setWindowIcon(QIcon(":/Images/Images/transferInquiryIcon-modified"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("存在输入为空或运行时间不为整数"));
        box.addButton(tr("确定"),QMessageBox::AcceptRole);
        if(box.exec()==QMessageBox::Accepted)
        {
            box.close();
        }
    }else{
//        //创建PCB，调用Controller里的函数
        int cnt=0;
        if (flag1&&!(ui->Time1lineEdit->text().isEmpty())){
            //flag1为true说明要么都为空要么都有数据,且数据正确，不为空则为有数据
            QString name=ui->Name1lineEdit->text();
            QString t=ui->Time1lineEdit->text();
            int time=t.toInt();
            QString p=ui->Pri1comboBox->currentText();
            QString s=ui->Size1lineEdit->text();
            int size=s.toInt();
            int pri=p.toInt();
            //PCB(QString name,int time,int pri,QString state);
            PCB new_p(name,time,pri,"waiting",-1,size);
            //这里还得写一个逻辑，判断后台是否创建成功，不能有相同名字的进程
            if(PcbController->creatProcess(new_p)){
                cnt++;
                ui->Name1lineEdit->clear();
                ui->Time1lineEdit->clear();
            }else{
                QMessageBox box;
                box.setWindowTitle(tr("输入错误——进程1"));
                // box.setWindowIcon(QIcon(":/Images/Images/transferInquiryIcon-modified"));
                box.setIcon(QMessageBox::Warning);
                box.setText(tr("进程名1重复，请更改进程名"));
                box.addButton(tr("确定"),QMessageBox::AcceptRole);
                if(box.exec()==QMessageBox::Accepted)
                {
                    box.close();
                }
            }
        }
        if (flag2&&!(ui->Time2lineEdit->text().isEmpty())){
            //flag2为true说明要么都为空要么都有数据,且数据正确，不为空则为有数据
            QString name=ui->Name2lineEdit->text();
            QString t=ui->Time2lineEdit->text();
            int time=t.toInt();
            QString p=ui->Pri2comboBox->currentText();
            QString s=ui->Size2lineEdit->text();
            int size=s.toInt();
            int pri=p.toInt();
            //PCB(QString name,int time,int pri,QString state);
            PCB new_p(name,time,pri,"waiting",-1,size);
            if(PcbController->creatProcess(new_p)){
                //成功创建
                cnt++;
                ui->Name2lineEdit->clear();
                ui->Time2lineEdit->clear();
            }else{
                QMessageBox box;
                box.setWindowTitle(tr("输入错误——进程2"));
                // box.setWindowIcon(QIcon(":/Images/Images/transferInquiryIcon-modified"));
                box.setIcon(QMessageBox::Warning);
                box.setText(tr("进程名2重复，请更改进程名"));
                box.addButton(tr("确定"),QMessageBox::AcceptRole);
                if(box.exec()==QMessageBox::Accepted)
                {
                    box.close();
                }
            }
        }
        if (flag3&&!(ui->Time3lineEdit->text().isEmpty())){
            //flag3为true说明要么都为空要么都有数据,且数据正确，不为空则为有数据
            QString name=ui->Name3lineEdit->text();
            QString t=ui->Time3lineEdit->text();
            int time=t.toInt();
            QString p=ui->Pri3comboBox->currentText();
            QString s=ui->Size3lineEdit->text();
            int size=s.toInt();
            int pri=p.toInt();
            //PCB(QString name,int time,int pri,QString state);
            PCB new_p(name,time,pri,tr("waiting"),-1,size);
            if(PcbController->creatProcess(new_p)){
                cnt++;
                ui->Name3lineEdit->clear();
                ui->Time3lineEdit->clear();
            }else{
                QMessageBox box;
                box.setWindowTitle(tr("输入错误——进程3"));
                // box.setWindowIcon(QIcon(":/Images/Images/transferInquiryIcon-modified"));
                box.setIcon(QMessageBox::Warning);
                box.setText(tr("进程名3重复，请更改进程名"));
                box.addButton(tr("确定"),QMessageBox::AcceptRole);
                if(box.exec()==QMessageBox::Accepted)
                {
                    box.close();
                }
            }
        }
        QString cntQ=QString::number(cnt);
        QMessageBox::information(this,tr("创建成功"),"成功创建"+cntQ+"个进程!");
        this->close();
        int a=PcbController->getWaitingList().length();
        qDebug()<<"waiting队列有"<<a<<"进程";
    }
}//确定按钮槽函数

void AddNewProcessDialog::on_ClearpushButton_clicked()
{
    ui->Name1lineEdit->clear();
    ui->Time1lineEdit->clear();
    ui->Name2lineEdit->clear();
    ui->Time2lineEdit->clear();
    ui->Name3lineEdit->clear();
    ui->Time3lineEdit->clear();

    ui->Size1lineEdit->clear();
    ui->Size2lineEdit->clear();
    ui->Size3lineEdit->clear();

    QMessageBox::information(this,tr("清空成功"),"已全部清空");
    qDebug()<<"点击清空"<<endl;
}

void AddNewProcessDialog::on_CancelpushButton_clicked()
{
    this->close();
    qDebug()<<"取消"<<endl;
}

Ui::AddNewProcessDialog *AddNewProcessDialog::getUi() const
{
    return ui;
}

void AddNewProcessDialog::setUi(Ui::AddNewProcessDialog *value)
{
    ui = value;
}
