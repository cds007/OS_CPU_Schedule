#include "queuedao.h"


QueueDao::QueueDao()
{
    if (QSqlDatabase::contains("os"))
    {
        database = QSqlDatabase::database("os");
        qDebug()<<"构造函数，连接成功!";
    }
    else
    {
        //连接是伴随程序运行而产生，终止而死亡
        database = QSqlDatabase::addDatabase("QSQLITE","os");
        database.setDatabaseName("OS.db");
        qDebug()<<"构造函数，重新连接!";
    }
    qDebug()<<QSqlDatabase::contains("os");
}

bool QueueDao::openDb()
{
    if (!database.open())
       {
           qDebug() << "Error: Failed to connect database." << database.lastError();
       }
       else
       {
           // do something
        qDebug()<<"打开数据库成功!";
       }

    return true;
}

bool QueueDao::isTableExist(QString &tableName)
{

        if(database.tables().contains(tableName))
        {
            qDebug()<<"数据表存在!";
            return true;
        }
        return false;
}

void QueueDao::queryTable(QVector<PCB> &p)
{

    QSqlQuery sqlQuery(database);
    sqlQuery.exec("SELECT * FROM waitingQueue");
    if(!sqlQuery.exec())
    {
      qDebug() << "Error: Fail to query table. " << sqlQuery.lastError();
    }
    else
    {
      while(sqlQuery.next())
     {
      QString name = sqlQuery.value(0).toString();
      int time = sqlQuery.value(1).toInt();
      int pri = sqlQuery.value(2).toInt();
      QString state = sqlQuery.value(3).toString();
      int startpos = sqlQuery.value(4).toInt();
      int size = sqlQuery.value(5).toInt();
      PCB new_p(name,time,pri,state,startpos,size);
      p.append(new_p);//P向量中添加PCB
      qDebug()<<QString("name:%1    time:%2    pri:%3    state:%4  startpos:%5 size:&6 ").arg(name).arg(time).arg(pri).arg(state).arg(startpos).arg(size);
      }
    }
}

void QueueDao::singleInsertData(PCB &singleData)
{
    QSqlQuery sqlQuery(database);
        sqlQuery.prepare("INSERT INTO waitingQueue VALUES(:name,:time,:pri,:state,:startpos,:size)");
        sqlQuery.bindValue(":name", singleData.getPcbName());
        sqlQuery.bindValue(":time", singleData.getPcbTime());
        sqlQuery.bindValue(":pri", singleData.getPriority());
        sqlQuery.bindValue(":state", singleData.getState());
        sqlQuery.bindValue(":startpos",singleData.getPcbStartPos());
        sqlQuery.bindValue("size",singleData.getPcbSize());
        if(!sqlQuery.exec())
        {
            qDebug() << "Error: Fail to insert data. " << sqlQuery.lastError();
        }
        else
        {
            // do something
            qDebug() << "插入成功";
        }
}

void QueueDao::deleteData(QString pname)
{
    QSqlQuery sqlQuery(database);
    //用占位符连接一个Int和QString
    sqlQuery.prepare(QString("DELETE FROM waitingQueue WHERE name = '%1'").arg(pname));
    //上述语句切记勿忘单引号
        if(!sqlQuery.exec())
        {
            qDebug()<<sqlQuery.lastError();
            qDebug()<<"删除失败";
        }
        else
        {
            qDebug()<<"deleted data success!";
        }
}

void QueueDao::closeDb()
{
    database.close();
}



