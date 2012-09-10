//
//  Thread.h
//  MobileSNG
//
//  Created by 박 진 on 12. 9. 6..
//
//

#pragma once

#include <queue>

#define THREAD_FUNC(X) (bool (Thread::*)(Thread*, void*))(&X)

//(bool (Thread::*)(void*))

struct ThreadFunc;

class Thread
{
private:
    bool                             m_isAlarm;
    std::queue<ThreadFunc>           *m_pqThread;
    
public:
    Thread(bool isAlarm = false);
    ~Thread();
    
public:
    void threadUpdate();
    
public:
    void addWork(ThreadFunc &thread);
    void popWork();
    void AllClear();
};

struct ThreadObject {
    bool (Thread::*pFunc)(Thread *t, void *p);
    void *parameter;
    Thread *thisClass;
    
    ThreadObject(Thread *t = NULL) : pFunc(NULL), parameter(NULL), thisClass(t){}
    ThreadObject(bool (Thread::*pFunc)(Thread *t, void *p), Thread *thisClass, void *parameter)
    {
        this->pFunc     = pFunc;
        this->parameter = parameter;
        this->thisClass = thisClass;
    }
};

struct ThreadFunc
{
    ThreadObject        work;
    ThreadObject        fail;
    ThreadObject        complete;
    
    ThreadFunc(){}
    
    ThreadFunc(ThreadObject *work, ThreadObject *fail, ThreadObject *complete)
    {
        if(work)        this->work      = *work;
        if(fail)        this->fail      = *fail;
        if(complete)    this->complete  = *complete;
    }
};
