#ifndef QUEUEDAO_H
#define QUEUEDAO_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QVector>
#include "pcb.h"

class QueueDao
{
public:
    QueueDao();

    // 打开数据库
    bool openDb(void);
    // 判断数据表是否存在
    bool isTableExist(QString& tableName);
    // 查询全部数据
    void queryTable(QVector<PCB> &p);
    // 插入数据
    void singleInsertData(PCB &singleData); // 插入单条数据
   //void moreInsertData(PCB &moreData); // 插入多条数据
    // 删除数据
    void deleteData(QString pname);
    // 关闭数据库
    void closeDb(void);
private:
    QSqlDatabase database;
};

#endif // QUEUEDAO_H
