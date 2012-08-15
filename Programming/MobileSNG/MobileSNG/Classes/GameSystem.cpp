//
//  GameSystem.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "GameSystem.h"

using namespace std;

GameSystem::GameSystem()
{
    m_pInfoMgr  = NULL;
    m_pMap      = NULL;
    m_pNetwork  = NULL;
}

GameSystem::~GameSystem()
{
    if(m_pInfoMgr != NULL)
    {
        delete m_pInfoMgr;
        m_pInfoMgr = NULL;
    }
}

bool GameSystem::initialize(const char* strDBFile)
{
    //생성 안될때 (메모리 넘침)에 대한 예외처리도 해줘
    
    int mapLevel = 1;
    
    m_pInfoMgr  = new ObjectInfoMgr(strDBFile);
    m_pMap      = new MapMgr(mapLevel);
    m_pNetwork  = new Network;
    
    //일단 대충. 임시적

    return true;
}

void GameSystem::Update(float fDelta)
{
//    m_pMap->UpdateObjects(m_pInfoMgr);
//
//    if(m_qHarvest.empty()) return;
//    
//    CURLcode code;
//    HARVEST_QUEUE *object = &(m_qHarvest.front());
//        
//    code = m_pNetwork->connectHttp(object->url.data(), &(object->data));
//    
//    if( code == CURLE_OK )
//        m_qHarvest.pop();
//    else
//    {
//        stdQueueAllClear(m_qHarvest);
//        //실패하면 뭘 띄워주든가 해야하지 않냐
//        //성공해도 뭘 해줘야할테고
//    }
}

MapMgr* GameSystem::GetMapMgr()
{
    return m_pMap;
}