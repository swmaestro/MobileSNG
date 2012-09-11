//
//  Field.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 29..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Field.h"

Field::Field(ObjectInMap *pObject)
                                                    : ObjectInMap(pObject)
{
    m_type      = OBJECT_TYPE_FIELD;
    m_pCrop     = NULL;
    m_pTimer    = NULL;
}

Field::~Field()
{
    SAFE_DELETE(m_pCrop);
    SAFE_DELETE(m_pTimer);
}

Crop* Field::addCrop(int id, int time, ObjectInfoMgr *pInfoMgr)
{
    if( m_pCrop != NULL )
    {
        printf("%s AddCrop Error, already exist", __FUNCTION__);
        return false;
    }
    
    m_pCrop = new  Crop(id, pInfoMgr);
    SAFE_DELETE(m_pTimer);
    m_pTimer = new Timer(time);
    
    m_pTimer->StartTimer();
    
    return m_pCrop;
}

bool Field::UpdateSystem()
{
    if(m_pCrop == NULL) return false;
    
    m_beforeState = m_state;
    
    ObjectInfo info = m_pCrop->GetInfo()->GetObjInfo();
    
    if(m_state < CROP_STATE_DONE && m_pTimer->CheckTimer(info.GetTime()) == false)
        m_state = static_cast<float>(m_pTimer->GetTime()) / info.GetTime() * CROP_STATE_DONE;
    
    if (m_beforeState != m_state)
        return true;
    
    return false;
}

void Field::removeCrop()
{
    if(m_pCrop != NULL)
    {
        delete m_pCrop;
        m_pCrop = NULL;
        
        m_state = -1;
    }
}

Crop* Field::GetCrop()
{
    return m_pCrop;
}

bool Field::hasCrop()
{
    return m_pCrop != NULL;
}

bool Field::isDone()
{
    if(m_pCrop == NULL) return false;
    return m_state == CROP_STATE_DONE;
}

bool Field::isConstruct()
{
    return (m_beforeState < CROP_STATE_DONE) && (m_state == CROP_STATE_DONE);
}