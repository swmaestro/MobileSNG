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
    BUILDING_STATE_UNDER_CONSTRUCTION,
    BUILDING_STATE_WORKING,
    BUILDING_STATE_NONE
};

class Building : public Timer, public ObjectInMap
{    
public:
    Building(ObjectInMap *pObject, int nowTime);
    ~Building();
    
public:
    void UpdateSystem(ObjectInfoMgr *pInfoMgr);
};