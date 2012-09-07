//
//  ThreadUse.h
//  MobileSNG
//
//  Created by 박 진 on 12. 9. 6..
//
//

#pragma once

class ThreadUse
{
protected:
    std::queue<bool (ThreadUse::*)()>   m_qServer;

public:
    ThreadUse() {}
    ~ThreadUse() {}
    
public:
    void threadUpdate()
    {
        while ((m_qServer.empty() == false))
        {
            bool (ThreadUse::*pFunc)(void) = m_qServer.front();
            
            (this->*pFunc)();
            
            sleep(10);
        }
    }
};