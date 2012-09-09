//
//  Thread.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 9. 6..
//
//

#include "Thread.h"
#include "CCCommon.h"

using namespace cocos2d;
using namespace std;

Thread::Thread(bool isAlarm) : m_pqThread(NULL)
{
    m_isAlarm = isAlarm;
    m_pqThread = new queue<ThreadFunc>;
}

Thread::~Thread()
{
    if(m_pqThread)
    {
        delete m_pqThread;
        m_pqThread = NULL;
    }
}

void Thread::threadUpdate()
{
    while ((m_pqThread->empty() == false))
    {
        ThreadFunc func = m_pqThread->front();

        bool (Thread::*pFunc)(Thread*,void *)   = func.work.pFunc;
        void *parameter                         = func.work.parameter;
        Thread *thisClass                       = func.work.thisClass;

        if((this->*pFunc)(thisClass, parameter) == false)
        {
            if(m_isAlarm)
                CCMessageBox("연결에 실패하였습니다", "Error");
            
            pFunc = func.fail.pFunc;
            parameter = func.fail.parameter;
            thisClass = func.fail.thisClass;
            
            if(pFunc)
                (this->*pFunc)(thisClass, parameter);

            AllClear();
            return;
        }

        pFunc       = func.complete.pFunc;
        parameter   = func.complete.parameter;
        thisClass   = func.complete.thisClass;
        
        if(pFunc)
            (this->*pFunc)(thisClass, parameter);
        
        
        m_pqThread->pop();
    }
}

void Thread::addWork(ThreadFunc &thread)
{
    m_pqThread->push(thread);
}

void Thread::popWork()
{
    m_pqThread->pop();
}

void Thread::AllClear()
{
    while (!m_pqThread->empty())
        m_pqThread->pop();
}