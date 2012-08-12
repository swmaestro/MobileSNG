//
//  Building.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Building.h"
#include <ctime>

Building::Building(ObjectInMap *pObject, int nowTime) : ObjectInMap(pObject)
{
    m_type = OBJECT_TYPE_BUILDING;
    m_pTimer = new Timer(nowTime);
    m_beforeState = pObject->m_state;
}

Building::~Building()
{
    delete m_pTimer;
}

void Building::UpdateSystem(ObjectInfoMgr *pInfoMgr)
{
    float           time = -1;
    BUILDING_INFO   info;
    
    if(pInfoMgr->searchInfo(m_id, &info) == false)
    {
        printf("%s <- Building Error. Can't search Building info", __FUNCTION__);
        
        return;
    }

    if( m_state < BUILDING_STATE_COMPLETE_CONSTRUCTION )
        time = static_cast<float>(info.buildTime);
    else if( m_state == BUILDING_STATE_COMPLETE_CONSTRUCTION )
        time = static_cast<float>(info.object.time);

    if(m_pTimer->CheckTimer(time))
    if(m_state == BUILDING_STATE_COMPLETE_CONSTRUCTION)
        m_pTimer->StartTimer();

    if( m_state < BUILDING_STATE_COMPLETE_CONSTRUCTION )
        m_state = static_cast<float>(m_pTimer->GetTime()) / (time/4.f);
    else
        m_state = static_cast<float>(m_pTimer->GetTime()) / time + 4.f;
    
    if( m_beforeState != m_state )
        STATE_CHANGE_CALLBACK(this, m_state);
    
    m_beforeState = m_state;
}

Timer* Building::GetTimer()
{
    return m_pTimer;
}