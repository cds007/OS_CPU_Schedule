#ifndef PCB_H
#define PCB_H

#include <QObject>

class PCB
{
private:
   QString pcbName;//名字
   int pcbTime;//运行时间
   int priority;//优先级，数值越大优先级越高
   int HasRunning=0;//已经运行的时间
   QString state;//状态，有waiting,ready,running,hanging,terminate五种状态
   PCB* pcbPointer;

   //扩展PCB:11.15
   int pcbSize;//所需主存大小
   int pcbStartPos;//主存起始位置


public:
    PCB();
    PCB(QString name,int time,int pri,QString state,PCB* pointer);
    PCB(QString name,int time,int pri,QString state);
    PCB(QString name,int time,int pri,QString state,int pcbStartPos,int pcbSize);
    //按alt+enter自动生成set和get方法
    QString getPcbName() const;
    void setPcbName(const QString &value);
    int getPcbTime() const;
    void setPcbTime(int value);
    int getPriority() const;
    void setPriority(int value);
    QString getState() const;
    void setState(const QString &value);
    int getHasRunning() const;
    void setHasRunning(int value);
    int getPcbSize() const;
    void setPcbSize(int value);
    int getPcbStartPos() const;
    void setPcbStartPos(int value);
};

#endif // PCB_H
