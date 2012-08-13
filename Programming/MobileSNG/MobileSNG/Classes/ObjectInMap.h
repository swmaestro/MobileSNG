//
//  ObjectInMap.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 31..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "DataType.h"
#include "ObjectInfoMgr.h"

typedef int objectState;
#define STATE_CHANGE_CALLBACK(pObj, State) \
        if(m_pStateChangeCallBack)m_pStateChangeCallBack(pObj, State);

enum OBJECT_DIRECTION
{
    OBJECT_DIRECTION_LEFT,
    OBJECT_DIRECTION_RIGHT
};

enum OBJECT_TYPE
{
    OBJECT_TYPE_NONE,
    OBJECT_TYPE_BUILDING,
    OBJECT_TYPE_FIELD,
    OBJECT_TYPE_CROP = OBJECT_TYPE_FIELD,
    OBJECT_TYPE_ORNAMENT
};

class ObjectInMap
{
public:
    objectState             m_state;
    POINT<int>              m_position;
    SIZE<int>               m_size;
    OBJECT_DIRECTION        m_direction;
    OBJECT_TYPE             m_type;
    int                     m_id;
    
public:
    ObjectInMap(objectState      state      = -1, 
                POINT<int>       position   = POINT<int>(0,0),
                SIZE<int>        size       = SIZE<int>(0,0),
                OBJECT_DIRECTION dir        = OBJECT_DIRECTION_LEFT,
                int              id         = -1);
    ObjectInMap(const ObjectInMap *pObject);
    
    virtual ~ObjectInMap();
    
public:
    bool isIn(int x, int y, int w, int h);
    bool isIn(ObjectInMap *pObject);
    
public:
    virtual void UpdateSystem(ObjectInfoMgr *pInfoMgr);
};