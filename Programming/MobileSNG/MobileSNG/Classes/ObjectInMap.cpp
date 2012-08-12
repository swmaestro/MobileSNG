//
//  ObjectInMap.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 31..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "ObjectInMap.h"

ObjectInMap::ObjectInMap(objectState state, POINT<int> position, SIZE<int> size, OBJECT_DIRECTION dir, int id, void (*stateChangeCAllBack)(ObjectInMap* pObj, objectState state))
{
    m_state         = state;
    m_position      = position;
    m_size          = size;
    m_direction     = dir;
    m_id            = id;
    m_pStateChangeCallBack = stateChangeCAllBack;
}

ObjectInMap::ObjectInMap(const ObjectInMap *pObject)
{
    m_state         = pObject->m_state;
    m_position      = pObject->m_position;
    m_size          = pObject->m_size;
    m_direction     = pObject->m_direction;
    m_id            = pObject->m_id;
}

ObjectInMap::~ObjectInMap()
{
    
}

bool ObjectInMap::isIn(int x, int y, int w, int h)
{
    if( x > m_position.x + m_size.width )  return false;
    if( x + w < m_position.x )             return false;
    if( y > m_position.y + m_size.height ) return false;
    if( y + h < m_position.y )             return false;
        
    return true;
}

bool ObjectInMap::isIn(ObjectInMap *pObject)
{
    if( pObject->m_position.x > m_position.x + m_size.width )  
        return false;
    if( pObject->m_position.x + pObject->m_size.width < m_position.x )             
        return false;
    if( pObject->m_position.y > m_position.y + m_size.height ) 
        return false;
    if( pObject->m_position.y + pObject->m_size.height < m_position.y )             
        return false;
    
    return true;
}

void ObjectInMap::UpdateSystem(ObjectInfoMgr *pInfoMgr)
{
    //null
}