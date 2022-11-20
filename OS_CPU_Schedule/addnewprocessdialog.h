#ifndef ADDNEWPROCESSDIALOG_H
#define ADDNEWPROCESSDIALOG_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>

#include "allpcbcontroller.h"

namespace Ui {
class AddNewProcessDialog;
}

class AddNewProcessDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AddNewProcessDialog(QWidget *parent = nullptr);
    ~AddNewProcessDialog();


    void setPcbControllerPointer(AllPcbController *p);//设置PCB管理指针
    void InitComboBox();//初始化comboBox
    Ui::AddNewProcessDialog *getUi() const;
    void setUi(Ui::AddNewProcessDialog *value);

private slots:
    void on_SurepushButton_clicked();

    void on_ClearpushButton_clicked();

    void on_CancelpushButton_clicked();

private:
    Ui::AddNewProcessDialog *ui;

    AllPcbController *PcbController;
};

#endif // ADDNEWPROCESSDIALOG_H
