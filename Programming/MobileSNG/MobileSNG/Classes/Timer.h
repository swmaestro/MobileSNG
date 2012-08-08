//
//  BuildingObject.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "DataType.h"
#include "ObjectInfoMgr.h"

class Timer
{    
private:
    int                     m_innerTimer;
    int                     m_saveTime;
    bool                    m_isUse;
    
protected:
    int                     m_nowTime;
    
public:
    Timer(int nowTime);
    virtual ~Timer();

protected:
    bool _CheckTimer(int completeTime);
    
public:
    int     GetTime();
    void    StartTimer();
    bool    GetisUse();
};