//
//  BuildingObject.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ObjectInfoMgr.h"

class Timer
{    
private:
    long long int                     m_innerTimer;
    long long int                     m_saveTime;
    bool                              m_isUse;
    long long int                     m_nowTime;
    
public:
    Timer(long long int nowTime);
    virtual ~Timer();

public:
    bool CheckTimer(long long int completeTime);
    
public:
    long long int     GetTime();
    void    StartTimer();
    bool    GetisUse();
    void    SetTime(long long int t);
};