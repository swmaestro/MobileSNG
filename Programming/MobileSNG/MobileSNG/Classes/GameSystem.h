//
//  GameSystem.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ObjectInfoMgr.h"
#include "MapMgr.h"
#include "Network.h"
#include "Player.h"
#include "ObjectIndexMgr.h"
#include <string>

class GameSystem
{
private:
    ObjectInfoMgr                   *m_pInfoMgr;
    MapMgr                          *m_pMap;
    Network                         *m_pNetwork;
    Player                          *m_pPlayer;
    ObjectIndexMgr                  *m_pIdxMgr;
    
private:
    std::vector<ObjectInMap*>::iterator m_objectIter;
    
public:
    GameSystem(const char* strDBFile, int & mapLevel);
    ~GameSystem();
    
public:
    CommonInfo* GetCommonInfo(ObjectInMap *pObj);
    ObjectInfo GetObjectInfo(ObjectInMap *pObj);
    
    CommonInfo* GetCommonInfo(int type, int id);
    ObjectInfo GetObjectInfo(int type, int id);
    
private:
    bool _PostResourceInfo(int gold, int cash, int exp);
    
public:
    //물건을 살수있는지 여부를 묻는 그런 함수의 이름
    bool    isUseObject(CommonInfo *pCommonInfo);
    bool    isUseObject(ObjectInMap* pObj);
    
public:
    bool    BuyObject(ObjectInMap *pObj);
    void    SellObject(ObjectInMap *pObj);
    
public:
    bool Harvest(POINT<int> &pos, ObjectInMap **ppOut);
    bool Harvest(ObjectInMap **ppObject);
    void AllHarvest();
    void FastComplete(ObjectInMap *pObject);
    
public:
    bool init();
    bool UpdateMapObject(ObjectInMap **ppOut);
    
public:
    bool            addObject(ObjectInMap *pInfo, int time);
    bool            moveObject(POINT<int> &pos, ObjectInMap *obj2);
    bool            addCrop(Field *pField, int id, int time);
    void            removeCrop(Field *pField);
    void            removeObject(POINT<int> &pos);
    bool            isObjectInMap(POINT<int> pos);
    bool            isObjectInMap(POINT<int> pos, SIZE<int> size);

    ObjectInMap*                FindObject(POINT<int> pos);
    std::vector<ObjectInMap*>   FindObjects(POINT<int> pos, SIZE<int> size);
    
public:
    inline ObjectInfoMgr*   GetInfoMgr()    { return m_pInfoMgr; }
//    inline MapMgr*          GetMapMgr()     { return m_pMap;     }
    inline Player*          GetPlayer()     { return m_pPlayer;    }
};