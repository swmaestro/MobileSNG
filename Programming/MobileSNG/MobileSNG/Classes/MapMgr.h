//
//  MapMgr.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 29..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Field.h"
#include "Ornament.h"
#include "Building.h"
#include <vector>

template <typename TYPE>
inline bool intersectBoxWithBox(POINT<TYPE> pos1, SIZE<TYPE> size1, 
                                POINT<TYPE> pos2, SIZE<TYPE> size2)
{
    if( pos2.x > pos1.x + size1.width )
        return false;
    if( pos2.x + size2.width < pos1.x )
        return false;
    if( pos2.y > pos1.y + size1.height )
        return false;
    if( pos2.y + size2.height < pos2.y )
        return false;
    
    return true;
}

template <typename TYPE>
inline bool intersectBoxWithPoint(POINT<TYPE> boxPos, SIZE<TYPE> boxSize, POINT<TYPE> pos)
{
    if( (boxPos.x < pos.x && pos.x < boxPos.x + boxSize.width) &&
       (boxPos.y < pos.y && pos.y < boxPos.y + boxSize.height) )
        return true;
    
    return false;
}

class MapMgr
{
private:
    std::vector<ObjectInMap*>           m_vObjects;
    int                                 m_mapLevel;
    
public:  
    MapMgr(int mapLevel);
    ~MapMgr();
    
private:
    bool                   _isInMap(POINT<int> &pos, SIZE<int> size);
    static ObjectInMap*    _CreateObject(ObjectInMap *obj, int time = 0);
    
public:
    void        UpdateObjects(ObjectInfoMgr *pInfoMgr);
    
public:
    bool        addObject(ObjectInMap &info, int time);
    bool        moveObject(POINT<int> &pos, ObjectInMap *obj2);
    
public:
//    bool        addCrop(Field *pField, int id, int time);
//    void        removeCrop(Field *pField);
    
public:
    void        removeObject(int index);
    void        removeObject(ObjectInMap *pOut);
    void        removeObject(POINT<int> &pos);
    void        removeObjects(POINT<int> &pos, SIZE<int> &size);
    
public:
    ObjectInMap*                FindObject(POINT<int> pos);
    std::vector<ObjectInMap*>   FindObjects(POINT<int> pos, SIZE<int> size);
    bool                        isObjectInMap(POINT<int> pos);
    bool                        isObjectInMap(POINT<int> pos, SIZE<int> size);
    
public:
    int         GetMapLevel();
};