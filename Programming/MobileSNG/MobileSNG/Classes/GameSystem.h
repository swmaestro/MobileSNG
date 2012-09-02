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
#include "CommonVillage.h"
#include "Player.h"
#include "ObjectIndexMgr.h"
#include "DateInfo.h"
#include <string>
#include "rapidxml.hpp"

//struct RESOURCE {
//    int exp;
//    int money;
//    int cash;
//    
//    RESOURCE(int exp = 0, int money = 0, int cash = 0)
//    {
//        this->exp   = exp;
//        this->money = money;
//        this->cash  = cash;
//    }
//};

class GameSystem :private CommonVillage
{
private:
    ObjectInfoMgr                   *m_pInfoMgr;
    MapMgr                          *m_pMap;
    Network                         *m_pNetwork;
    Player                          *m_pPlayer;
    ObjectIndexMgr                  *m_pIdxMgr;    
    
public:
    GameSystem(const char* strDBFile, int & mapLevel, Network *pNetwork);
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
    bool    SellObject(ObjectInMap *pObj);

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
    bool            _removeNetworkObject(const char *userID, int index);
    void            _removeCrop(Field *pField);
    void            _removeObject(POINT<int> &pos);
    void            _removeObject(ObjectInMap *pObj);
    
public:
    bool            addObject(ObjectInMap *pObj, int time, int index = -1);
    bool            moveObject(POINT<int> &pos, ObjectInMap *obj2, OBJECT_DIRECTION dir = OBJECT_DIRECTION_LEFT);
    bool            addCrop(Field *pField, int id, int time, int index = -1);
    bool            isObjectInMap(POINT<int> pos);
    bool            isObjectInMap(POINT<int> pos, SIZE<int> size);

    ObjectInMap*                FindObject(POINT<int> pos);
    std::vector<ObjectInMap*>   FindObjects(POINT<int> pos, SIZE<int> size);
    
public:
    bool            SetUpVillageList(bool isUpdate = true);
    
public:
    inline ObjectInfoMgr*   GetInfoMgr()    { return m_pInfoMgr; }
//    inline MapMgr*          GetMapMgr()     { return m_pMap;     }
    inline Player*          GetPlayer()     { return m_pPlayer;    }
    inline std::vector<ObjectInMap*>& GetAllObject()
    { return m_pMap->GetAllObject(); }
};