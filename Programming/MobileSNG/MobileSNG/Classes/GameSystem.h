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
#include "ThreadUse.h"

class GameSystem :private CommonVillage, public ThreadUse
{
private:
    MapMgr                              *m_pMap;
    Network                             *m_pNetwork;
    Player                              *m_pPlayer;
    ObjectIndexMgr                      *m_pIndexMgr;
    
public:
    GameSystem(const char* strDBFile, int & mapLevel, Network *pNetwork);
    ~GameSystem();
    
public:
    CommonInfo* GetCommonInfo(ObjectInMap *pObj);
    ObjectInfo GetObjectInfo(ObjectInMap *pObj);

    CommonInfo* GetCommonInfo(int type, int id);
    ObjectInfo GetObjectInfo(int type, int id);

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
    
public:
    bool init();

private:
    bool            _newObject(int objID, int index, POINT<int> position, OBJECT_DIRECTION dir);
    bool            _newCrop(int fieldIndex, int cropID);

    bool            _removeNetworkObject(ObjectInMap *pObject);
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
    
public:
    bool            addObject(ObjectInMap *pObj, int time, int index = -1);
    bool            changeObject(POINT<int> &pos, ObjectInMap *obj2, OBJECT_DIRECTION dir = OBJECT_DIRECTION_LEFT);
    bool            addCrop(Field *pField, int id, int time, bool isAdd = false);
    bool            isObjectInMap(POINT<int> pos);
    bool            isObjectInMap(POINT<int> pos, SIZE<int> size);
    bool            buildingConstructCheck(int index);

    ObjectInMap*                FindObject(POINT<int> pos);
    std::vector<ObjectInMap*>   FindObjects(POINT<int> pos, SIZE<int> size);
    
public:
    bool            SetUpVillageList(bool isUpdate = true);
    
    bool            test() { printf("Yeah! \n"); return true; }
    
public:
    inline ObjectInfoMgr*   GetInfoMgr()    { return m_pInfoMgr; }
    inline Player*          GetPlayer()     { return m_pPlayer;    }
    inline std::vector<ObjectInMap*>& GetAllObject()
    { return m_pMap->GetAllObject(); }
};