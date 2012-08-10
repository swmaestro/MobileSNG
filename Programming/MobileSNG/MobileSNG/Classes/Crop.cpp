//
//  Crop.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Crop.h"

Crop::Crop(int id, int time) : Timer(time)
{
    m_id    = id;
    m_state = CROP_STATE_GROW_1;
}

Crop::~Crop()
{
    
}

void Crop::UpdateSystem(ObjectInfoMgr *pInfoMgr)
{    
    CROP_INFO       info;
    
    if(pInfoMgr->searchInfo(m_id, &info) == false)
    {
        printf("%s <- Crop Error. Can't search Crop info", __FUNCTION__);
        return;
    }
    
    if(Timer::_CheckTimer(info.object.time) == false)
            m_state = m_nowTime / (info.object.time / 4);
    else    m_state = CROP_STATE_DONE;

}

objectState Crop::GetState()
{
    return m_state;
}

int Crop::GetID()
{
    return m_id;
}