//
//  ObjectInMap.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 31..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

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
    OBJECT_TYPE_CROP,
    OBJECT_TYPE_ORNAMENT
};

class ObjectInMap
{
protected:
    SIZE<int>               m_size;
    OBJECT_DIRECTION        m_direction;
    OBJECT_TYPE             m_type;
    int                     m_id;
    int                     m_index;
    objectState             m_beforeState;
    
public:
    objectState             m_state;
    POINT<int>              m_position;
    
public:
    ObjectInMap();
    ObjectInMap(objectState state, POINT<int> position, SIZE<int> size, OBJECT_DIRECTION dir, int id, int index = -1);
    ObjectInMap(const ObjectInMap *pObject);
    
    virtual ~ObjectInMap();
    
public:
    bool isIn(int x, int y, int w, int h);
    bool isIn(ObjectInMap *pObject);
    
public:
    virtual bool UpdateSystem();
    
public:
    objectState             GetState();
    virtual bool            isDone();
    
public:
    SIZE<int>               GetSize();
    OBJECT_DIRECTION        GetDirection();
    OBJECT_TYPE             GetType();
    int                     GetID();
    
public:
    void                    SetIndex(int i);
    int                     GetIndex();

    inline void SetPosition(POINT<int> pos) { m_position = pos; }
    inline POINT<int>              GetPosition() { return m_position; }
    
    void                    SetType(OBJECT_TYPE type) { m_type = type; }
    
    void                    SetDirection(OBJECT_DIRECTION dir) { m_direction = dir; }
    
public:
    virtual bool isConstruct() { return false; }
};