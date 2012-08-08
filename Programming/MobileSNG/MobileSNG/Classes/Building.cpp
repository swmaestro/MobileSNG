//
//  Building.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Building.h"
#include <ctime>

Building::Building(ObjectInMap *pObject, int nowTime)
               : Timer(nowTime), ObjectInMap(pObject)
{
    m_type = OBJECT_TYPE_BUILDING;
}

Building::~Building()
{
    
}

void Building::UpdateSystem(ObjectInfoMgr *pInfoMgr)
{
    int             time = -1;
    BUILDING_INFO   info;
    
    if(pInfoMgr->searchInfo(m_id, &info) == false)
    {
        printf("%s <- Building Error. Can't search Building info", __FUNCTION__);
        
        return;
    }

    if( m_state == BUILDING_STATE_UNDER_CONSTRUCTION )
        time = info.buildTime;
    else if( m_state == BUILDING_STATE_WORKING )
        time = info.object.time;

    if(Timer::_CheckTimer(time))
    {
        if( m_state == BUILDING_STATE_UNDER_CONSTRUCTION )
            StartTimer();

        m_state = m_state % 2 + 1;
    }
}