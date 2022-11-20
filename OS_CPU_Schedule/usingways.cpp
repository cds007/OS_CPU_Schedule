#include "usingways.h"
#include "ui_usingways.h"

UsingWays::UsingWays(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsingWays)
{
    ui->setupUi(this);
    setWindowTitle(tr("使用说明"));
    setWindowIcon(QIcon("Images/appHelpIcon-modified"));
    creatlistMainItem();

    connect(listWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(listWidgetChanged(QListWidgetItem*)));
}

UsingWays::~UsingWays()
{
    delete ui;
}

void UsingWays::creatlistMainItem(){
    QHBoxLayout *mainLayout = new QHBoxLayout;
    setLayout(mainLayout);

    listWidget = new QListWidget;
    listWidget->addItem(tr("关于开始运行与重新开始"));
    listWidget->addItem(tr("关于下一步"));
    listWidget->addItem(tr("关于算法切换"));
    listWidget->addItem(tr("关于自动/手动"));
    listWidget->addItem(tr("关于挂起"));
    listWidget->addItem(tr("关于添加进程"));
    listWidget->setFixedWidth(150);
    mainLayout->addWidget(listWidget);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    textBrowser = new QTextBrowser;
    textBrowser->setFont(QFont("consolas"));
    textBrowser->setOpenExternalLinks(true);
    textBrowser->setOpenLinks(true);
    rightLayout->addWidget(textBrowser);

    mainLayout->addLayout(rightLayout);
    listWidget->setCurrentItem(listWidget->item(0));
    listWidgetChanged(listWidget->item(0));
}

void UsingWays::listWidgetChanged(QListWidgetItem * item){
    if (listWidget->item(0) == item) {
        QFile txtFile("html/helpWatch.html");
        txtFile.open(QIODevice::ReadOnly);
        textBrowser->setHtml(txtFile.readAll());
        txtFile.close();
    }
    else if(listWidget->item(1)==item)
    {
        QFile txtFile("html/helpTransfer.html");
        txtFile.open(QIODevice::ReadOnly);
        textBrowser->setHtml(txtFile.readAll());
        txtFile.close();
    }
    else if(listWidget->item(2)==item)
    {
        QFile txtFile("html/helpAddLine.html");
        txtFile.open(QIODevice::ReadOnly);
        textBrowser->setHtml(txtFile.readAll());
        txtFile.close();
    }
    else if(listWidget->item(3)==item)
    {
        QFile txtFile("html/helpAddStation.html");
        txtFile.open(QIODevice::ReadOnly);
        textBrowser->setHtml(txtFile.readAll());
        txtFile.close();
    }
    else if(listWidget->item(4)==item)
    {
        QFile txtFile("html/helpAddConnection.html");
        txtFile.open(QIODevice::ReadOnly);
        textBrowser->setHtml(txtFile.readAll());
        txtFile.close();
    }
    else if(listWidget->item(5)==item)
    {
        QFile txtFile("html/helpAddByText.html");
        txtFile.open(QIODevice::ReadOnly);
        textBrowser->setHtml(txtFile.readAll());
        txtFile.close();
    }
}
