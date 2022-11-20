#ifndef MEMORYITEM_H
#define MEMORYITEM_H

#include<QString>

class MemoryItem
{
    //存储主存分区表的表目内容
public:
    MemoryItem();
    MemoryItem(int startpos,int length,int state);
    MemoryItem(QString pid,int startpos,int length,int state);
    int getStartpos() const;
    void setStartpos(int value);

    int getLength() const;
    void setLength(int value);

    int getState() const;
    void setState(int value);

    QString getPid() const;
    void setPid(const QString &value);

private:
    QString pid;//pid
    int startpos;//起址
    int length;//长度
    int state;//状态，1为占用，0为未占用
};

#endif // MEMORYITEM_H
