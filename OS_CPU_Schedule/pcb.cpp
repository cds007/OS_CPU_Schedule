#include "pcb.h"

QString PCB::getPcbName() const
{
    return pcbName;
}

void PCB::setPcbName(const QString &value)
{
    pcbName = value;
}

int PCB::getPcbTime() const
{
    return pcbTime;
}

void PCB::setPcbTime(int value)
{
    pcbTime = value;
}

int PCB::getPriority() const
{
    return priority;
}

void PCB::setPriority(int value)
{
    priority = value;
}

QString PCB::getState() const
{
    return state;
}

void PCB::setState(const QString &value)
{
    state = value;
}

int PCB::getHasRunning() const
{
    return HasRunning;
}

void PCB::setHasRunning(int value)
{
    HasRunning = value;
}

int PCB::getPcbSize() const
{
    return pcbSize;
}

void PCB::setPcbSize(int value)
{
    pcbSize = value;
}

int PCB::getPcbStartPos() const
{
    return pcbStartPos;
}

void PCB::setPcbStartPos(int value)
{
    pcbStartPos = value;
}

PCB::PCB()
{
    
}

PCB::PCB(QString name,int time,int pri,QString state,PCB* pointer){
    this->pcbName=name;
    this->pcbTime=time;
    this->priority=pri;
    this->state=state;
    this->pcbPointer=pointer;
}

PCB::PCB(QString name,int time,int pri,QString state){
    this->pcbName=name;
    this->pcbTime=time;
    this->priority=pri;
    this->state=state;
    this->pcbPointer=nullptr;
}

PCB::PCB(QString name, int time, int pri, QString state, int pcbStartPos, int pcbSize)
{
    this->pcbName=name;
    this->pcbTime=time;
    this->priority=pri;
    this->state=state;
    this->pcbPointer=nullptr;
    this->pcbSize=pcbSize;
    this->pcbStartPos=pcbStartPos;
}
