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
#include "User.h"
#include <string>


struct HARVEST_QUEUE {
    std::string          url;
    ObjectInMap         *pObject;
    CURL_DATA            data;
    
    HARVEST_QUEUE()
    {
        pObject = NULL;
    }

    HARVEST_QUEUE(char *url, ObjectInMap *pObject)
    {
        this->pObject   = pObject;
        this->url       = url;
    }
    
    ~HARVEST_QUEUE()
    {
        free(data.pContent);
    }
};

class GameSystem
{
private:
    ObjectInfoMgr                   *m_pInfoMgr;
    MapMgr                          *m_pMap;
    Network                         *m_pNetwork;
    User                            *m_pUser;
    
public:
    GameSystem(const char* strDBFile);
    ~GameSystem();
    
private:
    CommonInfo* _GetCommonInfo(ObjectInMap *pObj);
        
public:
    //물건을 살수있는지 여부를 묻는 그런 함수의 이름
    bool    isUseObject(CommonInfo *pCommonInfo);
    bool    isUseObject(ObjectInMap* pObj);
    
public:
    bool    BuyObject(ObjectInMap *pObj);
    void    SellObject(ObjectInMap *pObj);
    
public:
    bool Harvest(POINT<int> &pos, ObjectInMap *pOut);
    bool Harvest(ObjectInMap *pObject);
    
public:
    inline ObjectInfoMgr*   GetInfoMgr()    { return m_pInfoMgr; }
    inline MapMgr*          GetMapMgr()     { return m_pMap;    }
};