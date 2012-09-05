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

bool GameSystem::SellObject(ObjectInMap *pObj)
{
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
    
    CommonInfo *cominfo = GetCommonInfo(pObj);
    int price = cominfo->GetPrice();
    
    if( _removeObject(pObj) == false)
        return false;
    
    m_pPlayer->AddMoney(price/3);

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

bool GameSystem::_buildingConstruct(int index)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/build_check?id=%s&index=%d";
    char url[256];
    
    sprintf(url, baseURL, m_pPlayer->GetUserID(), index);
    
    CURL_DATA data;
    if( m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;

    return _networkNormalResult(&data);
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

bool GameSystem::_updateObject(int index, NetworkObject *pOut)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/onestateupdate?id=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_pPlayer->GetUserID(), index);

    CURL_DATA data;
    if( m_pNetwork->connectHttp(url, &data) != CURLE_OK )
        return false;

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

bool GameSystem::Harvest(POINT<int> &pos, ObjectInMap **ppOut)
{
    ObjectInMap *pObject = m_pMap->FindObject(pos);
    
    if(pObject == NULL)
        return false;
    
    *ppOut = pObject;
    
    return this->Harvest(&pObject);
}

bool GameSystem::Harvest(ObjectInMap **ppObject)
{
    if( *ppObject == NULL )
        return false;
    
    OBJECT_TYPE type = (*ppObject)->GetType();
    
    if( type == OBJECT_TYPE_ORNAMENT )  return false;
    if( (*ppObject)->isDone() == false) return false;
        
    int money, cash, exp;
    ObjectInfo info = GetObjectInfo((*ppObject));
    money   = info.GetExp();
    cash    = info.GetReward();
    exp     = info.GetCash();
    
    if( type == OBJECT_TYPE_BUILDING )
    {
        
    }
    
    return true;
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
    //아마 여기에 슬슬 서버연동이나 이런 선 작업들이 들어갈거야.
    
    if( SetUpVillageList() == false)
        return false;
    
    return true;
}

int GameSystem::_findFieldTime(int index, std::vector< std::pair<int, int> > *pvData)
{
    if(pvData == NULL) return -1;
    
    vector< pair<int, int> >::iterator iter;
    
    for(iter = pvData->begin(); iter != pvData->end(); ++iter)
    {
        if((*iter).first == index)
            return (*iter).second;
    }
    
    return -1;
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

bool GameSystem::addObject(ObjectInMap *pObj, int time, int index)
{
    if(pObj->GetType() == OBJECT_TYPE_CROP)
        return false;
    
    if(index == -1)
    {
        ObjectInMap *pCreatedObject = NULL;

        int idx;
        
        if(pObj->GetType() == OBJECT_TYPE_BUILDING)
             idx = m_pIndexMgr->buildingIndex();
        else idx = m_pIndexMgr->fieldIndex();
        
        if(idx == -1)
        {
            printf("%s <- Index Full\n", __FUNCTION__);
            return false;
        }
        
        pObj->SetIndex(idx);
        
        if( (pCreatedObject = m_pMap->addObject(pObj, m_pInfoMgr, 0)) )
        {
            if(pObj->GetType() == OBJECT_TYPE_BUILDING)
                 m_pIndexMgr->addBuildingIndex(idx);
            else m_pIndexMgr->addFieldIndex(idx);
        }
        else return false;
        
        if(_newObject(pObj->GetID(), pObj->GetIndex(), pObj->GetPosition(), pObj->GetDirection()) == false)
        {
            //서버에서 실패한거니까, 서버 통해서 재거하지 말고 클라 자체에서 제거하면되.
            m_pMap->removeObject(pObj->GetIndex());
            
            if(pObj->GetType() == OBJECT_TYPE_BUILDING)
                 m_pIndexMgr->removeBuildIndex(idx);
            else m_pIndexMgr->removeFieldIndex(idx);

            return false;
        }
        
        return true;
    }
    
    pObj->SetIndex(index);
    
    if(m_pMap->addObject(pObj, m_pInfoMgr, time) == false)
    {
        if(pObj->GetType() == OBJECT_TYPE_BUILDING)
             m_pIndexMgr->removeBuildIndex(index);
        else m_pIndexMgr->removeFieldIndex(index);

        return false;
    }
    
    return true;
}

bool GameSystem::changeObject(POINT<int> &pos, ObjectInMap *obj2, OBJECT_DIRECTION dir)
{
    if( obj2->GetType() == OBJECT_TYPE_CROP ) return false;
    
    POINT<int> tmpPos = obj2->GetPosition();
    OBJECT_DIRECTION tmpDir = obj2->GetDirection();
    
    if(m_pMap->moveObject(pos, obj2, dir) == false)
        return false;
    
    const char *baseURL = "http://swmaestros-sng.appspot.com/buildinglchange?id=%s&index=%d&location=%d&direction=%s";
    char url[256];
    string direction;
    
    if(dir == OBJECT_DIRECTION_LEFT)
        direction = "false";
    else direction = "true";
    
    int makePos = MAKEWORD(obj2->GetPosition().x, obj2->GetPosition().y);
    
    sprintf(url, baseURL, m_pPlayer->GetUserID(), obj2->GetIndex(), makePos, direction.data());
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error Moveojet\n", __FUNCTION__);
        m_pMap->moveObject(tmpPos, obj2, tmpDir);
        
        return false;
    }
    
    if(_networkNormalResult(&data) == false)
    {
        m_pMap->moveObject(tmpPos, obj2, tmpDir);
        return false;
    }
    
    return true;
}

bool GameSystem::addCrop(Field *pField, int id, int time, bool isAdd)
{
    int fieldIndex = pField->GetIndex();
    
    if( pField->addCrop(id, time, m_pInfoMgr) == NULL )
        return false;
    
    if(isAdd)
    {
        if(_newCrop(fieldIndex, id) == false)
        {
            pField->removeCrop();
            return false;
        }
    }
    
    return true;
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

bool GameSystem::SetUpVillageList(bool isUpdate)
{
    const char *baseURL;
    
    if(isUpdate)baseURL = "http://swmaestros-sng.appspot.com/vbstateupdate?id=%s";
    else        baseURL = "http://swmaestros-sng.appspot.com/vbinfolist?id=%s";
    
    char url[256];
    
    sprintf(url, baseURL, m_pPlayer->GetUserID());
    
    CURL_DATA buildingData;
    if(m_pNetwork->connectHttp(url, &buildingData) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    baseURL = "http://swmaestros-sng.appspot.com/crop_rlist?id=%s";
    sprintf(url, baseURL, m_pPlayer->GetUserID());
    
    CURL_DATA cropData;
    if(m_pNetwork->connectHttp(url, &cropData) != CURLE_OK)
        return false;
    
    vector< pair<ObjectInMap, long long int> > vBuild = _parseBuildingInVillage(buildingData.pContent);
    vector< pair<int, int> > vCrop = _parseCropInVillage(cropData.pContent);
    vector< pair<int, int> > vFieldTime;
    
    for(vector< pair<ObjectInMap, long long int> >::iterator
        iter = vBuild.begin(); iter != vBuild.end(); ++iter)
    {
        //밭과 건물을 비교하는 방법. index비교.
        ObjectInMap *pObject = &(*iter).first;
        long long int time = (*iter).second;
        
        addObject(pObject, time, pObject->GetIndex());
                
        if(pObject->GetType() == OBJECT_TYPE_FIELD)
        {
            pair<int, int> value(pObject->GetIndex(), time);
            vFieldTime.push_back(value);
        }
    }
    
    for(vector< pair<int, int> >::iterator iter = vCrop.begin(); iter != vCrop.end(); ++iter)
    {
        int fieldIndex  = (*iter).first;
        int cropID      = (*iter).second;
        
        Field *pField = dynamic_cast<Field*>(m_pMap->FindObject(fieldIndex));
        addCrop(pField, cropID, _findFieldTime(pField->GetIndex()));
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