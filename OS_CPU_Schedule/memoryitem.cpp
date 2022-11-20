#include "memoryitem.h"

MemoryItem::MemoryItem()
{

}

MemoryItem::MemoryItem(int startpos, int length, int state)
{
    this->startpos=startpos;
    this->length=length;
    this->state=state;
}

MemoryItem::MemoryItem(QString pid, int startpos, int length, int state)
{
    this->pid=pid;
    this->startpos=startpos;
    this->length=length;
    this->state=state;
}

int MemoryItem::getStartpos() const
{
    return startpos;
}

void MemoryItem::setStartpos(int value)
{
    startpos = value;
}

int MemoryItem::getLength() const
{
    return length;
}

void MemoryItem::setLength(int value)
{
    length = value;
}

int MemoryItem::getState() const
{
    return state;
}

void MemoryItem::setState(int value)
{
    state = value;
}

QString MemoryItem::getPid() const
{
    return pid;
}

void MemoryItem::setPid(const QString &value)
{
    pid = value;
}
