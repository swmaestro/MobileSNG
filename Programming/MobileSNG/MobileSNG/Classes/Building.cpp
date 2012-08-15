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
    m_pTimer->StartTimer();
    m_isWorking = false;
}

Building::~Building()
{
    delete m_pTimer;
}

bool Building::UpdateSystem(ObjectInfoMgr *pInfoMgr)
{
    float           time = -1;
    BUILDING_INFO   info;
    
    objectState beforeState = m_state;
    
    if(pInfoMgr->searchInfo(m_id, &info) == false)
    {
        printf("%s <- Building Error. Can't search Building info", __FUNCTION__);
        
        return false;
    }

    if( m_state < BUILDING_STATE_WORKING )
        time = static_cast<float>(info.buildTime);
    else if( m_state == BUILDING_STATE_WORKING )
        time = static_cast<float>(info.object.time);

    if(m_pTimer->CheckTimer(time))
    {
        if (m_state < BUILDING_STATE_WORKING)
            m_state = BUILDING_STATE_WORKING;
        
        if(m_state == BUILDING_STATE_WORKING)
        {
            if (!m_isWorking)
            {
                m_pTimer->StartTimer();
                m_isWorking = true;
            }
            else
                m_state = BUILDING_STATE_DONE;
        }
    }

    if( m_state < BUILDING_STATE_WORKING )
        m_state = static_cast<float>(m_pTimer->GetTime()) / time * BUILDING_STATE_WORKING;
    
    if( beforeState != m_state )
        return true;
    
    return false;
}

Timer* Building::GetTimer()
{
    return m_pTimer;
}