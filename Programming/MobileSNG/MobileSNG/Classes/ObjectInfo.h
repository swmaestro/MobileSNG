//
//  ObjectInfoType.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 17..
//
//

#pragma once

class ObjectInfo
{
private:
    int     m_time;
    int     m_reward;
    int     m_exp;
    int     m_cash;
    
public:
    ObjectInfo()
    {
        m_time = 0;
        m_reward = 0;
    }
    
    ObjectInfo(int _time, int _reward, int _exp, int _cash)
    {
        m_time        = _time;
        m_reward      = _reward;
        m_exp         = _exp;
        m_cash        = _cash;
    }
    
public:
    inline int GetTime()    { return m_time;    }
    inline int GetReward()  { return m_reward;  }
    inline int GetExp()     { return m_exp;     }
    inline int GetCash()    { return m_cash;    }
};

