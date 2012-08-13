//
//  Building.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Timer.h"
#include "ObjectInMap.h"

enum BUILDING_STATE
{
    BUILDING_STATE_UNDER_CONSTRUCTION_1,
    BUILDING_STATE_UNDER_CONSTRUCTION_2,
    BUILDING_STATE_COMPLETE_CONSTRUCTION,
    BUILDING_STATE_WORKING,
    BUILDING_STATE_NONE
};

class Building : public ObjectInMap
{
private:
    Timer           *m_pTimer;
    objectState      m_beforeState;
    void           (*m_pStateChangeCallBack)(ObjectInMap*, objectState);
    
public:
    Building(ObjectInMap *pObject, int nowTime, void (*stateChangeCAllBack)(ObjectInMap* pObj, objectState state) = NULL);
    ~Building();
    
public:
    void UpdateSystem(ObjectInfoMgr *pInfoMgr);
    Timer* GetTimer();
};