//
//  Building.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Building.h"
#include <ctime>

Building::Building(ObjectInMap *pObject, int nowTime, ObjectInfoMgr *pInfoMgr) : ObjectInMap(pObject)
{
    m_type = OBJECT_TYPE_BUILDING;
    m_pTimer = new Timer(nowTime);
    m_pTimer->StartTimer();
    m_isWork = false;
    m_isFriend = false;
    
    if(pObject->m_state == BUILDING_STATE_OTEHR_WORKING)
    {
        m_state = BUILDING_STATE_WORKING;
        m_isFriend = true;
    }
    
    if(pInfoMgr->searchInfo(m_id, &m_pInfo) == false)
        printf("%s <- Error, Can't find Building Information\n", __FUNCTION__);
}

Building::~Building()
{
    delete m_pTimer;
}

bool Building::UpdateSystem()
{
    double           time = -1;
    
    m_beforeState = m_state;
    
    if( m_state < BUILDING_STATE_WORKING)
        time = static_cast<float>(m_pInfo->GetBuildTime());
    else if( m_state == BUILDING_STATE_WORKING )
        time = static_cast<float>(m_pInfo->GetObjInfo().GetTime());
    
    if(m_pTimer->CheckTimer(time))
    {
        if (m_state < BUILDING_STATE_WORKING)
            m_state = BUILDING_STATE_WORKING;
        
        if(m_state == BUILDING_STATE_WORKING)
        {
            if(!m_isWork)
            {
                m_pTimer->StartTimer();
                m_isWork = true;
            }
            else m_state = BUILDING_STATE_DONE;
        }
    }

    if( m_state < BUILDING_STATE_COMPLETE_CONSTRUCTION )
        m_state = static_cast<double>(m_pTimer->GetTime()) / time * BUILDING_STATE_COMPLETE_CONSTRUCTION;
    
    if( m_beforeState != m_state)
        return true;
    
    if (m_needSync)
    {
        m_needSync = false;
        return true;
    }
    
    return false;
}

Timer* Building::GetTimer()
{
    return m_pTimer;
}

bool Building::isDone()
{
    return m_state == BUILDING_STATE_DONE;
}

bool Building::isFriend()
{
    return m_isFriend;
}

bool Building::isConstruct()
{
    return (m_beforeState < BUILDING_STATE_WORKING) && (m_state == BUILDING_STATE_WORKING);
}