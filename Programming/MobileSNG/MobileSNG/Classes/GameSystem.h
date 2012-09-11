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
#include "NetworkObject.h"
#include <queue>
#include "Thread.h"

class Map;

struct ADDOBJECT
{
    ObjectInMap obj;
    int time;
    int index;
    Map *pMap;
    
    ADDOBJECT() : time(0), index(-1), pMap(NULL) {}
    ADDOBJECT(ObjectInMap obj, int time, int index, Map *pMap)
    { this->obj = obj; this->time = time; this->index = index; this->pMap = pMap; }
    ADDOBJECT(ObjectInMap *pObj, int time, int index, Map *pMap)
    { this->obj = *pObj; this->time = time; this->index = index; this->pMap = pMap; }
};

struct CHANGEOBJECT
{
    POINT<int>   pos;
    ObjectInMap *pObj;
    OBJECT_DIRECTION dir;
    
    CHANGEOBJECT() : pos(0,0), pObj(NULL), dir(OBJECT_DIRECTION_LEFT) {}
    CHANGEOBJECT(POINT<int> &pos, ObjectInMap *pObj, OBJECT_DIRECTION dir)
    { this->pos = pos; this->pObj = pObj; this->dir = dir; }
};

struct SELLOBJECT {
    Map             *pMap;
    ObjectInMap     *pObj;
    
    SELLOBJECT() : pMap(NULL), pObj(NULL) {}
    SELLOBJECT(Map *pMap, ObjectInMap *pObj)
    {
        this->pMap  = pMap;
        this->pObj  = pObj;
    }
};

struct ADDCROP
{
    Field *pField;
    int id;
    int time;
    bool isAdd;
    Map *pMap;
    
    ADDCROP() : pField(NULL), id(-2), time(0), isAdd(false), pMap(NULL){}
    ADDCROP(Field *pField, int id, int time, bool isAdd, Map *pMap)
    {this->pField = pField; this->id = id; this->time = time; this->isAdd = isAdd; this->pMap = pMap;}
};

class GameSystem :private CommonVillage, public Thread
{
private:
    MapMgr                              *m_pMap;
    Network                             *m_pNetwork;
    Player                              *m_pPlayer;
    ObjectIndexMgr                      *m_pIndexMgr;
    
public:
    bool                                 m_isInit;
    
public:
    GameSystem(const char* strDBFile, int & mapLevel, Network *pNetwork);
    ~GameSystem();
    
private:
    bool            _newObject(int objID, int index, POINT<int> position, OBJECT_DIRECTION dir);
    bool            _newCrop(int fieldIndex, int cropID);

    bool            _removeNetworkObject(ObjectInMap *pObject);
    bool            _removeNetworkObject(int index);
    bool            _removeObject(POINT<int> &pos);
    bool            _removeObject(ObjectInMap *pObj);    

    bool            _buildingProductCheck(int index, bool isFriend = false);
    bool            _buildingProductComplete(int index);
    bool            _cropComplete(int fieldIndex);
    bool            _cropFailCheck(int fieldIndex);
    
    bool            _friendProductCheck(int index);
    bool            _friendProductComplete(int index);
    
    bool            _friendProduct(Building *pObject);
    bool            _singleProduct(Building *pObject);

    bool            _updateObject(int index, NetworkObject *pOut);
    
    bool            _updateUserInfo(Thread *t, void *parameter);
    
private:
    bool            _addObject(Thread* t, void *parameter);
    bool            _changeObject(Thread* t, void *parameter);
    bool            _addCrop(Thread* t, void *parameter);
    bool            _buildingConstructCheck(Thread* t, void *parameter);
    bool            _SetUpVillageList(Thread* t, void *parameter);
    bool            _SellObject(Thread* t, void *parameter);
    bool            _Harvest(Thread* t, void *parameter);
    
    bool            _FailObject(Thread* t, void *parameter);
    bool            _FailCrop(Thread* t, void *parameter);
    bool            _CompSell(Thread *t, void *parameter);
    
public:
    bool init();

public:
    bool            SetUpVillageList(bool isUpdate = true);
    
public:
    void            addObject(ObjectInMap *pObj, Map *pMap, int time, int index = -1, bool isThread = false);
    void            changeObject(POINT<int> &pos, ObjectInMap *obj2, OBJECT_DIRECTION dir = OBJECT_DIRECTION_LEFT, bool isThread = true);
    void            addCrop(Field *pField, Map *pMap, int id, int time, bool isAdd = false, bool isThread = false);
    void            buildingConstructCheck(int index);

    bool            BuyObject(ObjectInMap *pObj);
    void            SellObject(ObjectInMap *pObj, Map *pMap, ThreadObject complete, ThreadObject fail, bool isThread = true);
    
    bool            VillageUpdate();

public:
    void Harvest(POINT<int> &pos, ObjectInMap **ppOut, ThreadObject complete, ThreadObject fail);
    
public:
    //물건을 살수있는지 여부를 묻는 그런 함수의 이름
    bool    isUseObject(CommonInfo *pCommonInfo);
    bool    isUseObject(ObjectInMap* pObj);
    
public:
    ObjectInMap*                FindObject(POINT<int> pos);
    std::vector<ObjectInMap*>   FindObjects(POINT<int> pos, SIZE<int> size);

    bool            isObjectInMap(POINT<int> pos);
    bool            isObjectInMap(POINT<int> pos, SIZE<int> size);
    
public:
    CommonInfo*         GetCommonInfo(ObjectInMap *pObj);
    ObjectInfo          GetObjectInfo(ObjectInMap *pObj);
    
    CommonInfo*         GetCommonInfo(int type, int id);
    ObjectInfo          GetObjectInfo(int type, int id);
    
    
public:
    inline ObjectInfoMgr*   GetInfoMgr()    { return m_pInfoMgr; }
    inline Player*          GetPlayer()     { return m_pPlayer;    }
    inline std::vector<ObjectInMap*>& GetAllObject()
    { return m_pMap->GetAllObject(); }
};