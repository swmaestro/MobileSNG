//
//  GameSystem.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "GameSystem.h"
#include "CCCommon.h"

using namespace std;

GameSystem::GameSystem(const char* strDBFile, int & mapLevel)
{
    m_pInfoMgr  = new ObjectInfoMgr();
    m_pInfoMgr->loadData(strDBFile);
    m_pMap      = new MapMgr(mapLevel);
    m_pNetwork  = new Network;
    m_pUser     = new User;
    m_pUser->UpdateData(m_pNetwork);
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

CommonInfo* GameSystem::GetCommonInfo(ObjectInMap *pObj)
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

ObjectInfo GameSystem::GetObjectInfo(ObjectInMap *pObj)
{
    if( pObj->GetType() == OBJECT_TYPE_BUILDING )
    {
        BuildingInfo *pInfo;
        if(m_pInfoMgr->searchInfo(pObj->GetID(), &pInfo))
            return pInfo->GetObjInfo();
    }
    else if( pObj->GetType() == OBJECT_TYPE_CROP )
    {
        CropInfo *pInfo;
        if(m_pInfoMgr->searchInfo(pObj->GetID()
                                  , &pInfo))
            return pInfo->GetObjInfo();
    }
    
    return ObjectInfo();
}

bool GameSystem::BuyObject(ObjectInMap *pObj)
{
    if(m_pUser->AddMoney(-GetCommonInfo(pObj)->GetPrice()) == false)
        return false;
    return true;
}

void GameSystem::SellObject(ObjectInMap *pObj)
{
    m_pUser->AddMoney(-GetCommonInfo(pObj)->GetPrice());
}

bool GameSystem::isUseObject(CommonInfo *pCommonInfo)
{
    return m_pUser->GetLevel() >= pCommonInfo->GetLevel();
}

bool GameSystem::isUseObject(ObjectInMap* pObj)
{
   return m_pUser->GetLevel() >= GetCommonInfo(pObj)->GetLevel();
}

bool GameSystem::Harvest(POINT<int> &pos, ObjectInMap **ppOut)
{
    ObjectInMap *pObject = m_pMap->FindObject(pos);
    
    if(pObject == NULL)
        return false;
    
    *ppOut = pObject;
    
    return this->Harvest(&pObject);
}

void GameSystem::AllHarvest()
{
//    vector<ObjectInMap*> vObjects = m_pMap->GetAllObject();
//    vector<ObjectInMap*>::iterator iter;
//    
//    int exp         = 0;
//    int money       = 0;
//    int harvestNum  = 0;
//    
//    ObjectInfo info;
//    
//    for(iter = vObjects.begin(); iter != vObjects.end(); ++iter)
//    {
//        if(Harvest(&*iter))
//        {
//            info = _GetObjectInfo(*iter);
//            exp += info.GetExp();
//            money += info.GetReward();
//            harvestNum++;
//        }
//    }
//    
//    exp *= 1.5f;
//    money *= 1.5f;
//    
//    m_pUser->AddMoney(money);
//    m_pUser->AddExp(exp);
//    m_pUser->AddCash(-(harvestNum * 100));
}

bool GameSystem::_PostResourceInfo(int gold, int cash, int exp)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/villageadder?id=%s&costA=%d&costB=%d&exp=%d";
    char url[256];
    
    char id[32];
    m_pUser->GetInfo(id, NULL, NULL);
    
    sprintf(url, baseURL, id, gold, cash, exp);
    
    CURL_DATA data;
    if( m_pNetwork->connectHttp(url, &data) != CURLE_OK )
        return false;

    return true;
}

void GameSystem::FastComplete(ObjectInMap *pObject)
{
    OBJECT_TYPE type = pObject->GetType();
    
    if( type == OBJECT_TYPE_BUILDING )
    {
        if(pObject->m_state == BUILDING_STATE_WORKING )
            pObject->m_state = BUILDING_STATE_DONE;
        else if(pObject->m_state <= BUILDING_STATE_UNDER_CONSTRUCTION_2)
            pObject->m_state = BUILDING_STATE_WORKING;
    }
    else if (type == OBJECT_TYPE_CROP )
    {
        if(pObject->m_state != CROP_STATE_DONE)
            pObject->m_state = CROP_STATE_DONE;
    }
    
    if(_PostResourceInfo(0, 0, -100))
        m_pUser->AddCash(-100);
}

bool GameSystem::Harvest(ObjectInMap **ppObject)
{    
    if( ppObject == NULL )
        return false;
    
    OBJECT_TYPE type = (*ppObject)->GetType();
    
    if( type == OBJECT_TYPE_ORNAMENT ) return false;
    
    
    int     exp         = 0;
    int     reward      = 0;
    bool    isDone      = false;
    
    exp     = GetObjectInfo((*ppObject)).GetExp();
    reward  = GetObjectInfo((*ppObject)).GetReward();
    
    if(type == OBJECT_TYPE_BUILDING){
    if(dynamic_cast<Building*>((*ppObject))->m_state == BUILDING_STATE_DONE)
        isDone = true;
    }
    else
    {
        Field *pField = dynamic_cast<Field*>(*ppObject);
        
        if(pField->GetCrop())
        if(pField->GetCrop()->GetState() == CROP_STATE_DONE)
            isDone = true;
    }

    if(isDone == false) return false;
    
    if( _PostResourceInfo(reward, 0, -exp) == false )
        return false;
    
    m_pUser->AddExp(exp);
    m_pUser->AddMoney(reward);
    
    if(type == OBJECT_TYPE_BUILDING)
    {
        Building * b = dynamic_cast<Building*>((*ppObject));
        b->m_state = BUILDING_STATE_WORKING;
        b->GetTimer()->StartTimer();
    }
    
    else // type == object_type_crop
    {
        Field *pField = static_cast<Field*>((*ppObject));
        pField->removeCrop();
    }
    
    return true;
}