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
    static ObjectInMap*    _CreateObject(ObjectInMap *obj, ObjectInfoMgr *pInfoMgr, int time = 0);
    
public:
    bool        addObject(ObjectInMap *pInfo, ObjectInfoMgr *pInfoMgr, int time);
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
    int             GetMapLevel();
//    ObjectInMap*    GetObject(int idx);
//    ObjectInMap*    GetObject(bool isNext = true);
    std::vector<ObjectInMap *> GetAllObject();
};

