//
//  GameSystem.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "GameSystem.h"
#include "CCCommon.h"
#include "Map.h"

using namespace std;
using namespace rapidxml;

GameSystem::GameSystem(const char* strDBFile, int & mapLevel, Network *pNetwork) : CommonVillage(pNetwork)
{
    m_pInfoMgr->loadData(strDBFile);
    m_pNetwork  = pNetwork;
    m_pIndexMgr   = new ObjectIndexMgr;
    m_pMap      = new MapMgr(mapLevel);
    m_pPlayer     = new Player(m_pNetwork);
}

GameSystem::~GameSystem()
{
    SAFE_DELETE(m_pMap);
    SAFE_DELETE(m_pNetwork);
    SAFE_DELETE(m_pPlayer);
    SAFE_DELETE(m_pIndexMgr);
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
        
        type    = OBJECT_TYPE_CROP;
        id      = pField->GetCrop()->GetID();
    }

    return GetObjectInfo(type, id);
}

CommonInfo* GameSystem::GetCommonInfo(int type, int id)
{
    if(type == OBJECT_TYPE_BUILDING || type == OBJECT_TYPE_FIELD)
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
    else // Field type
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
    if(m_pPlayer->AddMoney(-GetCommonInfo(pObj)->GetPrice()/3) == false)
        return false;
    return true;
}

bool GameSystem::isUseObject(CommonInfo *pCommonInfo)
{
    return m_pPlayer->GetLevel() >= pCommonInfo->GetLevel();
}

bool GameSystem::isUseObject(ObjectInMap* pObj)
{
   return m_pPlayer->GetLevel() >= GetCommonInfo(pObj)->GetLevel();
}



bool GameSystem::_buildingProductCheck(int index, bool isFriend)
{
    const char *baseURL;
    
    if (isFriend == false)  baseURL = "http://swmaestros-sng.appspot.com/product_check?id=%s&index=%d";
    else baseURL = "http://swmaestros-sng.appspot.com/friendp_check?id=%s&index=%d";

    char url[256];
    sprintf(url, baseURL, m_pPlayer->GetUserID(), index);

    CURL_DATA data;
    if( m_pNetwork->connectHttp(url, &data) != CURLE_OK )
        return false;
    
    return _networkNormalResult(&data);
}

bool GameSystem::_cropFailCheck(int fieldIndex)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/fail_check?id=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_pPlayer->GetUserID(), fieldIndex);
    
    if( m_pMap->FindObject(fieldIndex) == NULL )
        return false;
    
    CURL_DATA data;
    if( m_pNetwork->connectHttp(url, &data) != CURLE_OK )
        return false;
    
    return _networkNormalResult(&data);
}

bool GameSystem::_friendProductCheck(int index)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/brequest_productend?owner=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_pPlayer->GetUserID(), index);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    return _networkNormalResult(&data);
}

bool GameSystem::_friendProductComplete(int index)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/brequest_end?owner=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_pPlayer->GetUserID(), index);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    return _networkNormalResult(&data);
}

bool GameSystem::_updateObject(int index, NetworkObject *pOut)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/onestateupdate?id=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_pPlayer->GetUserID(), index);

    CURL_DATA data;
    if( m_pNetwork->connectHttp(url, &data) != CURLE_OK )
        return false;
    
    if( pOut == NULL ) return true;

    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pNode = xmlDoc.first_node()->first_node();
    int count = atoi(pNode->value());
    
    if(count == 0) return false;
    
    pNode = pNode->next_sibling()->first_node();
    
    pOut->index = atoi(pNode->value());
    pNode = pNode->next_sibling();
    pOut->id    = atoi(pNode->value());
    pNode = pNode->next_sibling();

    objectState state = atoi(pNode->value());
    _getObjectState(&state);
    pOut->state = state;
    
    pNode = pNode->next_sibling();

    int wordpos = atoi(pNode->value());
    pOut->position.x = GETWORD_X(wordpos);
    pOut->position.y = GETWORD_Y(wordpos);
    pNode = pNode->next_sibling();
    
    string direction    = pNode->value();
    
    if( strcmp(direction.data(), "false") == 0 )
         pOut->direction = OBJECT_DIRECTION_LEFT;
    else pOut->direction = OBJECT_DIRECTION_RIGHT;
    pNode = pNode->next_sibling();

    string date         = pNode->value();
    DateInfo dateInfo;
    dateInfo.UpdateDate(date.data());
    pOut->date = dateInfo;
    
    return true;
}

bool GameSystem::_friendProduct(Building *pObject)
{
    int index = pObject->GetIndex();
        
    if(_friendProductCheck(index)    == false)  return false;
    if(_friendProductComplete(index) == false)  return false;
        
    pObject->m_state = BUILDING_STATE_WORKING;
    pObject->GetTimer()->StartTimer();
        
    return true;
}

bool GameSystem::_singleProduct(Building *pObject)
{
    int index = pObject->GetIndex();
    
    if( _buildingProductCheck(index)    == false) return false;
    if( _buildingProductComplete(index) == false) return false;
    
    pObject->m_state = BUILDING_STATE_WORKING;
    pObject->GetTimer()->StartTimer();
        
    return true;
}

void GameSystem::Harvest(POINT<int> &pos, ObjectInMap **ppOut, ThreadObject complete, ThreadObject fail)
{
    ObjectInMap *pObject = m_pMap->FindObject(pos);
    
    if(pObject == NULL) return;
    
    *ppOut = pObject;
    
//    return this->_Harvest(&pObject);
    ThreadObject work(this);
    work.pFunc = (bool (Thread::*)(Thread*, void*))(&GameSystem::_Harvest);
    work.parameter = pObject;
    
    complete.parameter  = pObject;
//    fail.parameter      = pObject;
    static_cast<TALKBOX*>(fail.parameter)->pObj = pObject;
    
    ThreadFunc func(&work, &fail, &complete);
    addWork(func);
}

bool GameSystem::_buildingProductComplete(int index)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/ap_requestend?id=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_pPlayer->GetUserID(), index);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    return _networkNormalResult(&data);
}

bool GameSystem::init()
{
    if( SetUpVillageList() == false)
        return false;
    
    return true;
}


bool GameSystem::_newObject(int objID, int index, POINT<int> position, OBJECT_DIRECTION dir)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/buildinginsert?id=%s&bindex=%d&index=%d&location=%d&direction=%s";
    char url[256];
    
    string direction;
    if (dir == OBJECT_DIRECTION_LEFT)
        direction = "false";
    else    direction = "true";
    
    int makePos = MAKEWORD(position.x, position.y);
    sprintf(url, baseURL, m_pPlayer->GetUserID(), objID, index, makePos, direction.data());
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    return _networkNormalResult(&data);
}

bool GameSystem::_newCrop(int fieldIndex, int cropID)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/crop_request?id=%s&index=%d&cindex=%d";
    char url[256];
    sprintf(url, baseURL, m_pPlayer->GetUserID(), fieldIndex, cropID);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    return _networkNormalResult(&data);
}

bool GameSystem::_cropComplete(int fieldIndex)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/crop_rend?id=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_pPlayer->GetUserID(), fieldIndex);
    
    CURL_DATA data;
    if( m_pNetwork->connectHttp(url, &data) != CURLE_OK )
        return false;
    
    return _networkNormalResult(&data);
}

bool GameSystem::_removeNetworkObject(ObjectInMap *pObject)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/vbdelete?id=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_pPlayer->GetUserID(), pObject->GetIndex());
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    return _networkNormalResult(&data);
}

bool GameSystem::isObjectInMap(POINT<int> pos)
{
    return m_pMap->isObjectInMap(pos);
}

bool GameSystem::isObjectInMap(POINT<int> pos, SIZE<int> size)
{
    return m_pMap->isObjectInMap(pos, size);
}

ObjectInMap* GameSystem::FindObject(POINT<int> pos)
{
    return m_pMap->FindObject(pos);
}

std::vector<ObjectInMap*> GameSystem::FindObjects(POINT<int> pos, SIZE<int> size)
{
    return m_pMap->FindObjects(pos, size);
}

bool GameSystem::SetUpVillageList(bool isUpdate)
{
    //애는 딱히 돌릴필요는 없을거 같다.
    return _SetUpVillageList(this, &isUpdate);
}

void GameSystem::addObject(ObjectInMap *pObj, int time, int index, bool isThread)
{
    ThreadObject work(this), fail(this);
    work.pFunc      = (bool (Thread::*)(Thread*, void*))(&GameSystem::_addObject);
    work.parameter  = new ADDOBJECT(pObj, time, index);
    
    fail.pFunc      = (bool (Thread::*)(Thread*, void*))(&GameSystem::_Fail);
    fail.parameter  = (void*)("오브젝트 추가 오류");
    
    if(isThread)
    {
        ThreadFunc func(&work, &fail, NULL);
        addWork(func);
    }
    else
    {
        if(_addObject(this, work.parameter) == false)
            _Fail(this, fail.parameter);
    }
}

void GameSystem::changeObject(POINT<int> &pos, ObjectInMap *obj2, OBJECT_DIRECTION dir, bool isThread)
{
    ThreadObject work(this), fail(this);
    work.pFunc      = (bool (Thread::*)(Thread*, void*))(&GameSystem::_changeObject);
    work.parameter  = new CHANGEOBJECT(pos, obj2, dir);
    fail.pFunc      = (bool (Thread::*)(Thread*, void*))(&GameSystem::_Fail);
    fail.parameter  = (void*)("오브젝트 이동, 방향 변경 오류");
    
    if(isThread)
    {
        ThreadFunc func(&work, &fail, NULL);
        addWork(func);
    }
    else
    {
        if(_changeObject(this, work.parameter) == false)
            _Fail(this, fail.parameter);
    }
}

void GameSystem::addCrop(Field *pField, int id, int time, bool isAdd, bool isThread)
{
    ThreadObject work(this), fail(this);
    work.pFunc      = (bool (Thread::*)(Thread*, void*))(&GameSystem::_addCrop);
    work.parameter  = new ADDCROP(pField, id, time, isAdd);
    fail.pFunc      = (bool (Thread::*)(Thread*, void*))(&GameSystem::_Fail);
    fail.parameter  = (void*)("농작물 추가 오류");
    
    if(isThread)
    {
        ThreadFunc func(&work, &fail, NULL);
        addWork(func);
    }
    else
    {
        if(_addCrop(this, work.parameter) == false)
            _Fail(this, fail.parameter);
    }
}

void GameSystem::buildingConstructCheck(int index)
{
    ThreadObject work(this);
    work.pFunc      = (bool (Thread::*)(Thread*, void*))(&GameSystem::_buildingConstructCheck);
    work.parameter  = new int(index);
    
    ThreadFunc func(&work, NULL, NULL);
    
    addWork(func);
}

void GameSystem::SellObject(ObjectInMap *pObj, bool isThread)
{
    ThreadObject work(this);
    work.pFunc      = (bool (Thread::*)(Thread*, void*))(&GameSystem::_buildingConstructCheck);
    work.parameter  = pObj;
    
    if(isThread)
    {
        ThreadFunc func(&work, NULL, NULL);
        addWork(func);
    }
    else _SellObject(this, work.parameter);
}

bool GameSystem::_Harvest(Thread* t, void *parameter)
{
    GameSystem *thisClass = static_cast<GameSystem*>(t);
    ObjectInMap *pObject = static_cast<ObjectInMap*>(parameter);
    
    if( pObject == NULL )
        return false;
    
    OBJECT_TYPE type = pObject->GetType();
    
    if( type == OBJECT_TYPE_ORNAMENT )  return false;
    if( pObject->isDone() == false) return false;
    
    int money, cash, exp, index;
    ObjectInfo info = thisClass->GetObjectInfo(pObject);
    money   = info.GetExp();
    cash    = info.GetReward();
    exp     = info.GetCash();
    index   = pObject->GetIndex();
    
    if( thisClass->_updateObject(index, NULL) == false) return false;
    
    if( type == OBJECT_TYPE_BUILDING )
    {
        Building *pBuilding = dynamic_cast<Building*>(pObject);
        if( pBuilding->isFriend() )
        {
            if(thisClass->_friendProduct(pBuilding) == false)
                return false;
            else return true;
        }
        else
        {
            if(thisClass-> _singleProduct(pBuilding) == false) return false;
            else return true;
        }
    }
    else
    {
        thisClass->_cropFailCheck(index);
        if(thisClass->_cropComplete(index) == false)
            return false;
        else
        {
            Field *pField = dynamic_cast<Field*>(pObject);
            pField->removeCrop();
        }
    }
    
    return true;
}

bool GameSystem::_removeObject(ObjectInMap *pObj)
{
    int index = pObj->GetIndex();
    
    if(_removeNetworkObject(pObj))
    {
        m_pMap->removeObject(pObj);
        
        if(index >= 1000)   m_pIndexMgr->removeFieldIndex(index);
        else                m_pIndexMgr->removeBuildIndex(index);
        
        return true;
    }
    
    return false;
}

bool GameSystem::_removeObject(POINT<int> &pos)
{
    return _removeObject(m_pMap->FindObject(pos));
}

bool GameSystem::_addObject(Thread* t, void *parameter)
{
    GameSystem *pThisClass = static_cast<GameSystem*>(t);
    ADDOBJECT *pAddObject = static_cast<ADDOBJECT*>(parameter);
    
    ObjectInMap *pObj = pAddObject->pObj;
    int time = pAddObject->time;
    int index = pAddObject->index;
    
    delete pAddObject;
    
    if(pObj->GetType() == OBJECT_TYPE_CROP)
        return false;
    
    if(index == -1)
    {
        ObjectInMap *pCreatedObject = NULL;
        
        int idx;
        
        if(pObj->GetType() == OBJECT_TYPE_BUILDING)
            idx = pThisClass->m_pIndexMgr->buildingIndex();
        else idx = pThisClass->m_pIndexMgr->fieldIndex();
        
        if(idx == -1)
        {
            printf("%s <- Index Full\n", __FUNCTION__);
            return false;
        }
        
        pObj->SetIndex(idx);
        
        if( (pCreatedObject = pThisClass->m_pMap->addObject(pObj, pThisClass->m_pInfoMgr, 0)) )
        {
            if(pObj->GetType() == OBJECT_TYPE_BUILDING)
                pThisClass->m_pIndexMgr->addBuildingIndex(idx);
            else pThisClass->m_pIndexMgr->addFieldIndex(idx);
        }
        else return false;
        if(pThisClass->_newObject(pObj->GetID(), pObj->GetIndex(), pObj->GetPosition(), pObj->GetDirection()) == false)
        {
            //서버에서 실패한거니까, 서버 통해서 재거하지 말고 클라 자체에서 제거하면되.
            pThisClass->m_pMap->removeObject(pObj->GetIndex());
            
            if(pObj->GetType() == OBJECT_TYPE_BUILDING)
                pThisClass->m_pIndexMgr->removeBuildIndex(idx);
            else pThisClass->m_pIndexMgr->removeFieldIndex(idx);
            
            return false;
        }
        
        return true;
    }
    
    pObj->SetIndex(index);
    
    if(pThisClass->m_pMap->addObject(pObj, pThisClass->m_pInfoMgr, time) == false)
    {
        if(pObj->GetType() == OBJECT_TYPE_BUILDING)
            pThisClass->m_pIndexMgr->removeBuildIndex(index);
        else pThisClass->m_pIndexMgr->removeFieldIndex(index);
        
        return false;
    }
    
    return true;
}

bool GameSystem::_changeObject(Thread* t, void *parameter)
{
    GameSystem *pThisClass = static_cast<GameSystem*>(t);
    CHANGEOBJECT *pChangeObject = static_cast<CHANGEOBJECT*>(parameter);
    
    POINT<int> pos = pChangeObject->pos;
    ObjectInMap *obj2 = pChangeObject->pObj;
    OBJECT_DIRECTION dir = pChangeObject->dir;
    
    delete pChangeObject;
    
    if( obj2->GetType() == OBJECT_TYPE_CROP ) return false;
    
    POINT<int> tmpPos = obj2->GetPosition();
    OBJECT_DIRECTION tmpDir = obj2->GetDirection();
    
    if(pThisClass->m_pMap->moveObject(pos, obj2, dir) == false)
        return false;
    
    const char *baseURL = "http://swmaestros-sng.appspot.com/buildinglchange?id=%s&index=%d&location=%d&direction=%s";
    char url[256];
    string direction;
    
    if(dir == OBJECT_DIRECTION_LEFT)
        direction = "false";
    else direction = "true";
    
    int makePos = MAKEWORD(obj2->GetPosition().x, obj2->GetPosition().y);
    
    sprintf(url, baseURL, pThisClass->m_pPlayer->GetUserID(), obj2->GetIndex(), makePos, direction.data());
    
    CURL_DATA data;
    if(pThisClass->m_pNetwork->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error Moveojet\n", __FUNCTION__);
        pThisClass->m_pMap->moveObject(tmpPos, obj2, tmpDir);
        
        return false;
    }
    
    if(pThisClass->_networkNormalResult(&data) == false)
    {
        pThisClass->m_pMap->moveObject(tmpPos, obj2, tmpDir);
        return false;
    }
    
    return true;
}

bool GameSystem::_addCrop(Thread* t, void *parameter)
{
    GameSystem *pThisClass = static_cast<GameSystem*>(t);
    ADDCROP *pAddCrop = static_cast<ADDCROP*>(parameter);
    
    Field *pField = pAddCrop->pField;
    int id = pAddCrop->id;
    int time = pAddCrop->time;
    bool isAdd = pAddCrop->isAdd;
    
    delete pAddCrop;
    
    int fieldIndex = pField->GetIndex();
    
    if( pField->addCrop(id, time, pThisClass->m_pInfoMgr) == NULL )
        return false;
    
    if(isAdd)
    {
        if(pThisClass->_newCrop(fieldIndex, id) == false)
        {
            pField->removeCrop();
            return false;
        }
    }
    
    return true;
}

bool GameSystem::_buildingConstructCheck(Thread* t, void *parameter)
{
    GameSystem *pThisClass = static_cast<GameSystem*>(t);
    int *pIndex = static_cast<int*>(parameter);
    int index = *pIndex;
    delete pIndex;
    
    const char *baseURL = "http://swmaestros-sng.appspot.com/build_check?id=%s&index=%d";
    char url[256];
    
    sprintf(url, baseURL, pThisClass->m_pPlayer->GetUserID(), index);
    
    CURL_DATA data;
    if( pThisClass->m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    return pThisClass->_networkNormalResult(&data);
}

bool GameSystem::_SetUpVillageList(Thread* t, void *parameter)
{
    GameSystem *pThisClass = static_cast<GameSystem*>(t);
    bool isUpdate = *static_cast<bool*>(parameter);
    
    const char *baseURL;
    
    if(isUpdate)baseURL = "http://swmaestros-sng.appspot.com/vbstateupdate?id=%s";
    else        baseURL = "http://swmaestros-sng.appspot.com/vbinfolist?id=%s";
    
    char url[256];
    
    sprintf(url, baseURL, pThisClass->m_pPlayer->GetUserID());
    
    CURL_DATA buildingData;
    if(pThisClass->m_pNetwork->connectHttp(url, &buildingData) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    baseURL = "http://swmaestros-sng.appspot.com/crop_rlist?id=%s";
    sprintf(url, baseURL, pThisClass->m_pPlayer->GetUserID());
    
    CURL_DATA cropData;
    if(pThisClass->m_pNetwork->connectHttp(url, &cropData) != CURLE_OK)
        return false;
    
    vector< pair<ObjectInMap, long long int> > vBuild = pThisClass->_parseBuildingInVillage(buildingData.pContent);
    vector< pair<int, int> > vCrop = pThisClass->_parseCropInVillage(cropData.pContent);
    vector< pair<int, int> > vFieldTime;
    
    for(vector< pair<ObjectInMap, long long int> >::iterator
        iter = vBuild.begin(); iter != vBuild.end(); ++iter)
    {
        //밭과 건물을 비교하는 방법. index비교.
        ObjectInMap *pObject = &(*iter).first;
        long long int time = (*iter).second;
        
        pThisClass->addObject(pObject, time, pObject->GetIndex());
        
        if(pObject->GetType() == OBJECT_TYPE_FIELD)
        {
            pair<int, int> value(pObject->GetIndex(), time);
            vFieldTime.push_back(value);
        }
        else
        {
            int state = pObject->GetState();
            
            if( state == BUILDING_STATE_COMPLETE_CONSTRUCTION)
                if(pThisClass->_updateObject(pObject->GetIndex(), NULL) == false)
                {
                    int a = 5;
                    a=3;
                }
        }
    }
    
    for(vector< pair<int, int> >::iterator iter = vCrop.begin(); iter != vCrop.end(); ++iter)
    {
        int fieldIndex  = (*iter).first;
        int cropID      = (*iter).second;
        
        Field *pField = dynamic_cast<Field*>(pThisClass->m_pMap->FindObject(fieldIndex));
        pThisClass->addCrop(pField, cropID, pThisClass->_findFieldTime(pField->GetIndex()));
    }
    
    return true;
}

bool GameSystem::_SellObject(Thread* t, void *parameter)
{
    GameSystem *pThisClass = static_cast<GameSystem*>(t);
    ObjectInMap *pObj = static_cast<ObjectInMap*>(parameter);

    OBJECT_TYPE type = pObj->GetType();

    if( type == OBJECT_TYPE_BUILDING )
    {
        if(dynamic_cast<Building*>(pObj)->isFriend())
            return false;
    }
    
    else if( type == OBJECT_TYPE_FIELD )
    {
        if(dynamic_cast<Field*>(pObj)->GetCrop())
            return false;
    }
    
    CommonInfo *cominfo = pThisClass->GetCommonInfo(pObj);
    int price = cominfo->GetPrice();
    
    if( pThisClass->_removeObject(pObj) == false)
        return false;
    
    pThisClass->m_pPlayer->AddMoney(price/3);
    
    return true;
}

bool GameSystem::_updateUserInfo(bool isThread)
{

    return true;
}

bool GameSystem::_Fail(Thread* t, void *parameter)
{
    cocos2d::CCMessageBox((const char*)parameter, "Error");
    return true;
}