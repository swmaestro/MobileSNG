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

CommonInfo* GameSystem::GetCommonInfo(ObjectInMap *pObj)
{
    return GetCommonInfo(pObj->GetType(), pObj->GetID());
}

ObjectInfo GameSystem::GetObjectInfo(ObjectInMap *pObj)
{
    return GetObjectInfo(pObj->GetType(), pObj->GetID());
}

CommonInfo* GameSystem::GetCommonInfo(int type, int id)
{
    if(type == OBJECT_TYPE_BUILDING)
    {
        BuildingInfo *pInfo;
        if(m_pInfoMgr->searchInfo(id, &pInfo))
            return pInfo;
    }
    else if(type == OBJECT_TYPE_CROP)
    {
        CropInfo    *pInfo;
        if(m_pInfoMgr->searchInfo(id, &pInfo))
            return pInfo;
    }
    else // ornament type
    {
        OrnamentInfo *pInfo;
        if(m_pInfoMgr->searchInfo(id, &pInfo))
            return pInfo;
    }
    
    return NULL;
}

ObjectInfo GameSystem::GetObjectInfo(int type, int id)
{
    if( type == OBJECT_TYPE_BUILDING )
    {
        BuildingInfo *pInfo;
        if(m_pInfoMgr->searchInfo(id, &pInfo))
            return pInfo->GetObjInfo();
    }
    else if( type == OBJECT_TYPE_CROP )
    {
        CropInfo *pInfo;
        if(m_pInfoMgr->searchInfo(id, &pInfo))
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
    vector<ObjectInMap*> vObjects = m_pMap->GetAllObject();
    vector<ObjectInMap*>::iterator iter;
    
    int exp         = 0;
    int money       = 0;
    int harvestNum  = 0;
    
    ObjectInfo info;
    
    for(iter = vObjects.begin(); iter != vObjects.end(); ++iter)
    {
        if(Harvest(&*iter))
        {
            info = GetObjectInfo(*iter);
            exp += info.GetExp();
            money += info.GetReward();
            harvestNum++;
        }
    }
    
    exp *= 1.5f;
    money *= 1.5f;
    
    m_pUser->AddMoney(money);
    m_pUser->AddExp(exp);
    m_pUser->AddCash(-(harvestNum * 100));
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
    
    exp     = GetObjectInfo((*ppObject)).GetExp();
    reward  = GetObjectInfo((*ppObject)).GetReward();
    
    m_pUser->AddExp(exp);
    m_pUser->AddMoney(reward);
    
    if(type == OBJECT_TYPE_BUILDING)
    {
        if((*ppObject)->m_state == BUILDING_STATE_DONE)
        {
            Building * b = dynamic_cast<Building*>((*ppObject));
            b->m_state = BUILDING_STATE_WORKING;
            b->GetTimer()->StartTimer();
            return true;
        }
    }
    
    else // type == object_type_crop
    {
        Field *pField = static_cast<Field*>((*ppObject));
        if(pField->GetCrop())
            if(pField->GetCrop()->GetState() == CROP_STATE_DONE)
            {
                m_pUser->AddExp(GetObjectInfo(*ppObject).GetExp());
                
                dynamic_cast<Field*>((*ppObject))->removeCrop();
                return true;
            }
    }
    
    //임시
    //    HARVEST_QUEUE object(const_cast<char*>("http://"), pObject);
    //
    //    m_qHarvest.push(object);
    
    return false;
}