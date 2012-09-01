//
//  Crop.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Crop.h"

Crop::Crop(int id, int time, int index, ObjectInfoMgr *pInfoMgr)
{
    m_id        = id;
    m_state     = CROP_STATE_GROW_1;
    m_pTimer    = new Timer(time);
    m_pInfo     = NULL;
    m_index     = index;
    
    if(pInfoMgr->searchInfo(m_id, &m_pInfo) == false)
        printf("%s <- Error, Can not find Crop Information\n", __FUNCTION__);
}

Crop::~Crop()
{
    delete m_pTimer;
}

bool Crop::UpdateSystem()
{
    objectState beforeState = m_state;
    
    if(m_state < CROP_STATE_DONE && m_pTimer->CheckTimer(m_pInfo->GetObjInfo().GetTime()) == false)
            m_state = static_cast<float>(m_pTimer->GetTime()) / m_pInfo->GetObjInfo().GetTime() * CROP_STATE_DONE;
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

CropInfo Crop::GetInfo()
{
    return *m_pInfo;
}

int Crop::GetIndex()
{
    return m_index;
}