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
    int   m_time;
    int   m_reward;
    
public:
    ObjectInfo()
    {
        m_time = 0;
        m_reward = 0;
    }
    
    ObjectInfo(int _time, int _reward)
    {
        m_time        = _time;
        m_reward      = _reward;
    }
    
public:
    inline int GetTime()   { return m_time;    }
    inline int GetReward() { return m_reward;  }
};

