//
//  BuildingObject.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "StateSystem.h"
#include <ctime>

StateSystem::StateSystem(objectState state, int _time)
{
    m_time          = _time;
    m_innerTimer    =  time(NULL);
    m_state         = state;
}

StateSystem::~StateSystem()
{
    
}

void StateSystem::_update(int completeTime)
{
    m_time += time(NULL) - m_innerTimer;
    
    // 0 -> completeTime
    if( m_time > completeTime )
    {   
        //construction -> complete -> working -> complete
        m_state = m_state%2 + 1;
        m_time = 0;
    }
    
    m_innerTimer = time(NULL);
}

objectState StateSystem::GetState()
{
    return m_state;
}

bool StateSystem::isIn(int x, int y, int w, int h)
{
    
    
    return true;
}

int StateSystem::GetTime()
{
    return m_time;
}