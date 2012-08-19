//
//  GameSystem.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "GameSystem.h"

using namespace std;

GameSystem::GameSystem(const char* strDBFile, int & mapLevel)
{
    m_pInfoMgr  = new ObjectInfoMgr();
    m_pInfoMgr->loadData(strDBFile);
    m_pMap      = new MapMgr(mapLevel);
    m_pNetwork  = new Network;
    m_pUser     = new User;
}

GameSystem::~GameSystem()
{
    SAFE_DELETE(m_pInfoMgr);
    SAFE_DELETE(m_pMap);
    SAFE_DELETE(m_pNetwork);
    SAFE_DELETE(m_pUser);
}

//void GameSystem::Update(float fDelta)
//{
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
//}

CommonInfo* GameSystem::_GetCommonInfo(ObjectInMap *pObj)
{
    if(pObj->GetType() == OBJECT_TYPE_BUILDING)
    {
        BuildingInfo *pInfo;
        if(m_pInfoMgr->searchInfo(pObj->GetID(), &pInfo))
            return pInfo;
    }
    else if(pObj->GetType() == OBJECT_TYPE_CROP)
    {
        CropInfo    *pInfo;
        if(m_pInfoMgr->searchInfo(pObj->GetID(), &pInfo))
            return pInfo;
    }
    else // ornament type
    {
        OrnamentInfo *pInfo;
        if(m_pInfoMgr->searchInfo(pObj->GetID(), &pInfo))
            return pInfo;
    }

    return NULL;
}

bool GameSystem::BuyObject(ObjectInMap *pObj)
{
    if(m_pUser->AddMoney(-_GetCommonInfo(pObj)->GetPrice()) == false)
        return false;
    return true;
}

void GameSystem::SellObject(ObjectInMap *pObj)
{
    m_pUser->AddMoney(-_GetCommonInfo(pObj)->GetPrice());
}

//ObjectInMap* GameSystem::GetObject(bool isNext)
//{
//    static int idx = 0;
//    if(isNext)++idx;
//    return m_pMap->GetAllObject()[idx];
//}
//
//ObjectInMap* GameSystem::GetObject(int idx)
//{
//    return m_pMap->GetAllObject()[idx];
//}

//MapMgr* GameSystem::GetMapMgr()
//{
//    return m_pMap;
//}

bool GameSystem::isUseObject(CommonInfo *pCommonInfo)
{
    return m_pUser->GetLevel() >= pCommonInfo->GetLevel();
}

bool GameSystem::isUseObject(ObjectInMap* pObj)
{
   return m_pUser->GetLevel() >= _GetCommonInfo(pObj)->GetLevel();
}

bool GameSystem::Harvest(POINT<int> &pos, ObjectInMap *pOut)
{
    ObjectInMap *pObject = m_pMap->FindObject(pos);
    
    if(pObject == NULL)
        return false;
    
    pOut = pObject;
    
    return this->Harvest(pObject);
}

bool GameSystem::Harvest(ObjectInMap *pObject)
{
    if( pObject == NULL )
        return false;
    
    OBJECT_TYPE type = pObject->GetType();
    
    if(type == OBJECT_TYPE_BUILDING)
    {
        if(pObject->m_state == BUILDING_STATE_DONE)
        {
            Building * b = dynamic_cast<Building*>(pObject);
            b->m_state = BUILDING_STATE_WORKING;
            b->GetTimer()->StartTimer();

            //임시. 얻는 금액만큼 경험치로 준다
            m_pUser->AddExp(_GetCommonInfo(pObject)->GetPrice());
            return true;
        }
    }
    
    else if(type == OBJECT_TYPE_FIELD)
    {
        Field *pField = static_cast<Field*>(pObject);
        if(pField->GetCrop())
            if(pField->GetCrop()->GetState() == CROP_STATE_DONE)
            {
                CropInfo *pInfo;
                m_pInfoMgr->searchInfo(pField->GetCrop()->GetID(), &pInfo);
                m_pUser->AddExp(pInfo->GetPrice());
                
                dynamic_cast<Field*>(pObject)->removeCrop();
                return true;
            }
    }
    
    //임시
    //    HARVEST_QUEUE object(const_cast<char*>("http://"), pObject);
    //
    //    m_qHarvest.push(object);
    
    return false;
}