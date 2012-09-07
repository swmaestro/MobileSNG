//
//  ObjectInMap.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 31..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "ObjectInMap.h"

ObjectInMap::ObjectInMap()
{
    m_state         = -1;
    m_position      = POINT<int>(0,0);
    m_size          = SIZE<int>(0,0);
    m_direction     = OBJECT_DIRECTION_LEFT;
    m_id            = -1;
    m_index         = -1;
    m_beforeState   = -1;
}

ObjectInMap::ObjectInMap(objectState state, POINT<int> position, SIZE<int> size, OBJECT_DIRECTION dir, int id, int index)
{
    m_state         = state;
    m_position      = position;
    m_size          = size;
    m_direction     = dir;
    m_id            = id;
    m_index         = index;
    m_beforeState   = -1;
}

ObjectInMap::ObjectInMap(const ObjectInMap *pObject)
{
    m_state         = pObject->m_state;
    m_position      = pObject->m_position;
    m_size          = pObject->m_size;
    m_direction     = pObject->m_direction;
    m_id            = pObject->m_id;
    m_index         = pObject->m_index;
    m_beforeState   = pObject->m_beforeState;
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

bool ObjectInMap::UpdateSystem()
{
    return false;
}

SIZE<int> ObjectInMap::GetSize()
{
    return m_size;
}

OBJECT_DIRECTION ObjectInMap::GetDirection()
{
    return m_direction;
}

OBJECT_TYPE ObjectInMap::GetType()
{
    return m_type;
}

int ObjectInMap::GetID()
{
    return m_id;
}

objectState ObjectInMap::GetState()
{
    return m_state;
}

bool ObjectInMap::isDone()
{
    return false;
}

void ObjectInMap::SetIndex(int i)
{
    m_index = i;
}

int ObjectInMap::GetIndex()
{
    return m_index;
}