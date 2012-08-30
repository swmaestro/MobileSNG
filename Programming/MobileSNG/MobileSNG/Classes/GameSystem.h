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
#include "DateInfo.h"
#include <string>
#include "rapidxml.hpp"

enum NETWORK_OBJECT {
    NETWORK_OBJECT_CONSTRUCTION,
    NETWORK_OBJECT_WAITTING,
    NETWORK_OBJECT_WORKING,
    NETWORK_OBJECT_DONE,
    NETWORK_OBJECT_FAIL,
    NETWORK_OBJECT_OTHER_WATTING
};

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

private:
    bool            _newObject(const char *userID, int objID, int index, POINT<int> position, OBJECT_DIRECTION dir);
    bool            _networkNormalResult(rapidxml::xml_document<char> *pXMLDoc);
    bool            _removeNetworkObject(const char *userID, int index);
    std::vector< std::pair<ObjectInMap, long long int> > _parseObjectInVillage(const char* pContent);
    bool        _getServerTime(DateInfo *pInfo);
    
public:
    bool            addObject(ObjectInMap *pObj, int time, int index = -1);
    bool            moveObject(POINT<int> &pos, ObjectInMap *obj2, OBJECT_DIRECTION dir = OBJECT_DIRECTION_LEFT);
    bool            addCrop(Field *pField, int id, int time, int index = -1);
    void            removeCrop(Field *pField);
    void            removeObject(POINT<int> &pos);
    bool            isObjectInMap(POINT<int> pos);
    bool            isObjectInMap(POINT<int> pos, SIZE<int> size);

    ObjectInMap*                FindObject(POINT<int> pos);
    std::vector<ObjectInMap*>   FindObjects(POINT<int> pos, SIZE<int> size);
    
    
    
public:
    bool            UpdateVillageList(bool isUpdate = false);
    
public:
    inline ObjectInfoMgr*   GetInfoMgr()    { return m_pInfoMgr; }
//    inline MapMgr*          GetMapMgr()     { return m_pMap;     }
    inline Player*          GetPlayer()     { return m_pPlayer;    }
};