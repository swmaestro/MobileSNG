//
//  BuildingSystem.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "BuildingSystem.h"
#include <ctime>

BuildingSystem::BuildingSystem(objectState state, BUILDING_OBJECT objectInfo) 
: StateSystem(state, objectInfo.nowTime)
{
    m_objectInfo = objectInfo;
}

BuildingSystem::~BuildingSystem()
{
    
}

void BuildingSystem::update()
{
    StateSystem::_update(0);
    
    // ???!?!?!?!?!?!? 뭐하지?
}