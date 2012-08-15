//
//  Crop.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Crop.h"

Crop::Crop(int id, int time)
{
    m_id        = id;
    m_state     = CROP_STATE_GROW_1;
    m_pTimer    = new Timer(time);
}

Crop::~Crop()
{
    delete m_pTimer;
}

bool Crop::UpdateSystem(ObjectInfoMgr *pInfoMgr)
{    
    CROP_INFO       info;
    
    if(pInfoMgr->searchInfo(m_id, &info) == false)
    {
        printf("%s <- Crop Error. Can't search Crop info", __FUNCTION__);
        return false;
    }
    
    objectState beforeState = m_state;
    
    if(m_state < CROP_STATE_DONE && m_pTimer->CheckTimer(info.object.time) == false)
            m_state = static_cast<float>(m_pTimer->GetTime()) / info.object.time * CROP_STATE_DONE;
    else    m_state = CROP_STATE_DONE;
    
    if (beforeState != m_state)
        return true;
    
    return false;
}

objectState Crop::GetState()
{
    return m_state;
}

int Crop::GetID()
{
    return m_id;
}

Timer* Crop::GetTimer()
{
    return m_pTimer;
}