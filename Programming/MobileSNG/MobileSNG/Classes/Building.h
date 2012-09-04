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

#define FIELD_OBJECT_ID -1

enum BUILDING_STATE
{
    BUILDING_STATE_UNDER_CONSTRUCTION_1,
    BUILDING_STATE_UNDER_CONSTRUCTION_2,
    BUILDING_STATE_COMPLETE_CONSTRUCTION,
    BUILDING_STATE_WORKING,
    BUILDING_STATE_DONE,
    BUILDING_STATE_FAIL,
    BUILDING_STATE_OTEHR_WORKING
};

class Building : public ObjectInMap
{
private:
    BuildingInfo   *m_pInfo;
    bool            m_isWork;
    bool            m_isFriend;
    
private:
    Timer           *m_pTimer;
    
public:
    Building(ObjectInMap *pObject, int nowTime, ObjectInfoMgr *pInfoMgr);
    ~Building();

public:
    bool UpdateSystem();
    Timer* GetTimer();
    bool isDone();
    bool isFriend();
};