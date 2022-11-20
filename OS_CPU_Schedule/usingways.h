#ifndef USINGWAYS_H
#define USINGWAYS_H

#include <QWidget>
#include<QListWidget>
#include<QTextBrowser>
#include<QHBoxLayout>
#include<QFile>

namespace Ui {
class UsingWays;
}

class UsingWays : public QWidget
{
    Q_OBJECT

public:
    explicit UsingWays(QWidget *parent = nullptr);
    ~UsingWays();

    void creatlistMainItem();

private slots:
    void listWidgetChanged(QListWidgetItem*);

private:
    Ui::UsingWays *ui;

    QListWidget *listWidget;
    QTextBrowser *textBrowser;
};

#endif // USINGWAYS_H
