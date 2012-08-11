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
//120811 CA Appended : Set Type
    m_type    = OBJECT_TYPE_FIELD;
    
    m_pCrop   = NULL;
}

Field::~Field()
{
    if(m_pCrop != NULL)
    {
        delete m_pCrop;
        m_pCrop = NULL;
    }    
}

bool Field::addCrop(int id, int time)
{
    if( m_pCrop != NULL )
    {
        printf("%s AddCrop Error, already exist", __FUNCTION__);
        return false;
    }

    m_pCrop = new  Crop(id, time);
    
    return true;
}

void Field::UpdateSystem(ObjectInfoMgr *pInfoMgr)
{
    if(m_pCrop == NULL) return;
    
    m_pCrop->UpdateSystem(pInfoMgr);
    m_state = m_pCrop->GetState();
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