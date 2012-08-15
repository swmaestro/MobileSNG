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
#include <queue>
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

template <typename T>
static void stdQueueAllClear(std::queue<T> &stdQueue)
{
    while (stdQueue.empty()==false)
        stdQueue.pop();
}

class GameSystem
{
private:
    ObjectInfoMgr                   *m_pInfoMgr;
    MapMgr                          *m_pMap;
    Network                         *m_pNetwork;
//    std::queue<HARVEST_QUEUE>       m_qHarvest;
    
public:
    GameSystem();
    ~GameSystem();
    
public:
    bool        initialize(const char* strDBFile);
    
public:
    
    
public:
    bool        Harvest(POINT<int> &pos, ObjectInMap *pOut);
    bool        Harvest(ObjectInMap *pObject);
    
public:
    void        Update(float fDelta);
    
public:
    MapMgr*          GetMapMgr();
    ObjectInfoMgr*   GetInfoMgr() { return m_pInfoMgr; }
    Network*        GetNetwork() { return m_pNetwork; }
};