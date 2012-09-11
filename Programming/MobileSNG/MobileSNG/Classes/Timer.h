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
    double                     m_innerTimer;
    double                     m_saveTime;
    bool                              m_isUse;
    double                     m_nowTime;
    
public:
    Timer(double nowTime);
    virtual ~Timer();

public:
    bool CheckTimer(double completeTime);
    
public:
    double     GetTime();
    void    StartTimer();
    bool    GetisUse();
    void    SetTime(double t);
};