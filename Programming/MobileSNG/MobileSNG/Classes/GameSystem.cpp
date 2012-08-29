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
    m_pNetwork  = new Network;
    m_pIdxMgr   = new ObjectIndexMgr;
    m_pMap      = new MapMgr(mapLevel, m_pIdxMgr);
    m_pPlayer     = new Player(m_pNetwork);
}

GameSystem::~GameSystem()
{
    SAFE_DELETE(m_pInfoMgr);
    SAFE_DELETE(m_pMap);
    SAFE_DELETE(m_pNetwork);
    SAFE_DELETE(m_pPlayer);
    SAFE_DELETE(m_pIdxMgr);
}

CommonInfo* GameSystem::GetCommonInfo(ObjectInMap *pObj)
{
    return GetCommonInfo(pObj->GetType(), pObj->GetID());
}

ObjectInfo GameSystem::GetObjectInfo(ObjectInMap *pObj)
{
    int type    = pObj->GetType();
    int id      = pObj->GetID();

    if( type == OBJECT_TYPE_FIELD )
    {
        Field *pField = dynamic_cast<Field*>(pObj);
        return pField->GetCrop()->GetInfo().GetObjInfo();
    }

    return GetObjectInfo(type, id);
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
    if(m_pPlayer->AddMoney(-GetCommonInfo(pObj)->GetPrice()) == false)
        return false;
    return true;
}

void GameSystem::SellObject(ObjectInMap *pObj)
{
    m_pPlayer->AddMoney(-GetCommonInfo(pObj)->GetPrice());
}

bool GameSystem::isUseObject(CommonInfo *pCommonInfo)
{
    return m_pPlayer->GetLevel() >= pCommonInfo->GetLevel();
}

bool GameSystem::isUseObject(ObjectInMap* pObj)
{
   return m_pPlayer->GetLevel() >= GetCommonInfo(pObj)->GetLevel();
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
//    m_pPlayer->AddMoney(money);
//    m_pPlayer->AddExp(exp);
//    m_pPlayer->AddCash(-(harvestNum * 100));
}

bool GameSystem::_PostResourceInfo(int gold, int cash, int exp)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/villageadder?id=%s&costA=%d&costB=%d&exp=%d";
    char url[256];

    char id[32];
    m_pPlayer->GetInfo(id, NULL, NULL, NULL);
    
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
        m_pPlayer->AddCash(-100);
}

bool GameSystem::Harvest(ObjectInMap **ppObject)
{    
    if( ppObject == NULL )
        return false;
    
    OBJECT_TYPE type = (*ppObject)->GetType();
    
    if( type == OBJECT_TYPE_ORNAMENT ) return false;
    if((*ppObject)->isDone() == false) return false;

    //필드가 경험치를 가질리가 없잖아..
    
    ObjectInfo objInfo = GetObjectInfo((*ppObject));
    
    int     exp         = objInfo.GetExp();
    int     reward      = objInfo.GetReward();

    if( _PostResourceInfo(reward, 0, exp) == false )   return false;
        
    m_pPlayer->AddExp(exp);
    m_pPlayer->AddMoney(reward);
    
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

bool GameSystem::init()
{
    //아마 여기에 슬슬 서버연동이나 이런 선 작업들이 들어갈거야.
    m_objectIter = m_pMap->GetAllObject().begin();
    
    return true;
}

bool GameSystem::UpdateMapObject(ObjectInMap **ppOut)
{
    if( m_objectIter == m_pMap->GetAllObject().end() )
        m_objectIter = m_pMap->GetAllObject().begin();
    else ++m_objectIter;
    
    *ppOut = (*m_objectIter);
    
    return (*m_objectIter)->UpdateSystem();
}

bool GameSystem::addObject(ObjectInMap *pObj, int time)
{
   return m_pMap->addObject(pObj, m_pInfoMgr, time);
}

bool GameSystem::moveObject(POINT<int> &pos, ObjectInMap *obj2)
{
    return m_pMap->moveObject(pos, obj2);
}

bool GameSystem::addCrop(Field *pField, int id, int time)
{
    return m_pMap->addCrop(pField, id, time, m_pInfoMgr);
}

void GameSystem::removeCrop(Field *pField)
{
    m_pMap->removeCrop(pField);
}

void GameSystem::removeObject(POINT<int> &pos)
{
    m_pMap->removeObject(pos);
}

bool GameSystem::isObjectInMap(POINT<int> pos)
{
    return m_pMap->isObjectInMap(pos);
}

bool GameSystem::isObjectInMap(POINT<int> pos, SIZE<int> size)
{
    return isObjectInMap(pos, size);
}

ObjectInMap* GameSystem::FindObject(POINT<int> pos)
{
    return m_pMap->FindObject(pos);
}

std::vector<ObjectInMap*> GameSystem::FindObjects(POINT<int> pos, SIZE<int> size)
{
    return FindObjects(pos, size);
}