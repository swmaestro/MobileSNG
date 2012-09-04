//
//  Crop.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Crop.h"

Crop::Crop(int id, ObjectInfoMgr *pInfoMgr)
{
    m_id        = id;
    m_pInfo     = NULL;
    
    if(pInfoMgr->searchInfo(m_id, &m_pInfo) == false)
        printf("%s <- Error, Can not find Crop Information\n", __FUNCTION__);
}

Crop::~Crop()
{

}

int Crop::GetID()
{
    return m_id;
}

CropInfo* Crop::GetInfo()
{
    return m_pInfo;
}