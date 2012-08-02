//
//  BuildingSystem.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "StateSystem.h"
#include "ObjectInMapType.h"

typedef enum BUILDING_STATE
{
    BUILDING_STATE_UNDER_CONSTRUCTION,
    BUILDING_STATE_COMPLETE,
    BUILDING_STATE_WORKING
}BUILDING_STATE;

class BuildingSystem : public StateSystem
{
private:
    
public:
    BuildingSystem(objectState state);
    ~BuildingSystem();
    
public:
    void update();
    bool isIn(int x, int y, int w, int h);
};