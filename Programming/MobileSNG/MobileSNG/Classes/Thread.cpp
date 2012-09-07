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

Thread::Thread(bool isAlarm)
{
    m_isAlarm = isAlarm;
}

Thread::~Thread()
{
    
}

void Thread::threadUpdate()
{
    while ((m_qThread.empty() == false))
    {
        ThreadFunc func = m_qThread.front();

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
        
        
        m_qThread.pop();
    }
}

void Thread::addWork(ThreadFunc &thread)
{
    m_qThread.push(thread);
}

void Thread::popWork()
{
    m_qThread.pop();
}

void Thread::AllClear()
{
    while (!m_qThread.empty())
        m_qThread.pop();
}