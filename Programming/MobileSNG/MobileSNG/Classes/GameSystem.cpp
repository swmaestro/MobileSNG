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

GameSystem::GameSystem(const char* strDBFile, int & mapLevel, Network *pNetwork)
{
    m_pInfoMgr  = new ObjectInfoMgr();
    m_pInfoMgr->loadData(strDBFile);
    m_pNetwork  = pNetwork;
    m_pIdxMgr   = new ObjectIndexMgr;
    m_pMap      = new MapMgr(mapLevel);
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
//    OBJECT_TYPE type = pObject->GetType();
//    
//    if( type == OBJECT_TYPE_BUILDING )
//    {
//        if(pObject->m_state == BUILDING_STATE_WORKING )
//            pObject->m_state = BUILDING_STATE_DONE;
//        else if(pObject->m_state <= BUILDING_STATE_UNDER_CONSTRUCTION_2)
//            pObject->m_state = BUILDING_STATE_WORKING;
//    }
//    else if (type == OBJECT_TYPE_CROP )
//    {
//        if(pObject->m_state != CROP_STATE_DONE)
//            pObject->m_state = CROP_STATE_DONE;
//    }
//    
//    if(_PostResourceInfo(0, 0, -100))
//        m_pPlayer->AddCash(-100);
}

bool GameSystem::Harvest(ObjectInMap **ppObject)
{    
    if( ppObject == NULL )
        return false;
    
    OBJECT_TYPE type = (*ppObject)->GetType();
    
    if( type == OBJECT_TYPE_ORNAMENT ) return false;
    if( (*ppObject)->isDone() == false) return false;

    ObjectInfo objInfo = GetObjectInfo((*ppObject));
    
    int exp = objInfo.GetExp();
    int reward = objInfo.GetReward();
    
    if(_PostResourceInfo(reward, 0, exp) == false) return false;
    
    m_pPlayer->AddExp(exp);
    m_pPlayer->AddMoney(reward);
    
    if(type == OBJECT_TYPE_BUILDING)
    {
        Building *pBuilding = dynamic_cast<Building*>((*ppObject));
        pBuilding->m_state = BUILDING_STATE_WORKING;
        pBuilding->GetTimer()->StartTimer();
    }
    else
    {
        Field *pField = dynamic_cast<Field*>((*ppObject));
        pField->removeCrop();
    }
    
    return true;
}

bool GameSystem::init()
{
    //아마 여기에 슬슬 서버연동이나 이런 선 작업들이 들어갈거야.
    
    if( SetUpVillageList() == false)
        return false;
    
    return true;
}

//bool GameSystem::UpdateMapObject(ObjectInMap **ppOut)
//{
//    vector<ObjectInMap*> &v = m_pMap->GetAllObject();
//    int size = m_pMap->GetAllObject().size();
//    
//    if( size == false) return false;
//    if( ++m_nObjectLoop >= size )
//        m_nObjectLoop = 0;
//    
//    *ppOut = v[m_nObjectLoop];
//    
//    return v[m_nObjectLoop]->UpdateSystem();
//}

bool GameSystem::_networkNormalResult(xml_document<char> *pXMLDoc)
{
    const char *value = pXMLDoc->first_node()->first_node()->value();
    
    if(strcmp(value, "false") == 0)
        return false;
    
    return true;
}

bool GameSystem::_newObject(const char *userID, int objID, int index, POINT<int> position, OBJECT_DIRECTION dir)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/buildinginsert?id=%s&bindex=%d&index=%d&location=%d&direction=%s";
    char url[256];
    
    string direction;
    if (dir == OBJECT_DIRECTION_LEFT)
            direction = "false";
    else    direction = "true";
    
    int makePos = MAKEWORD(position.x, position.y);
    sprintf(url, baseURL, userID, objID, index, makePos, direction.data());
    
    printf("%s\n", url);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    return _networkNormalResult(&xmlDoc);
}

bool GameSystem::addObject(ObjectInMap *pObj, int time, int index)
{    
    if(pObj->GetType() == OBJECT_TYPE_CROP)
        return false;

    if(index == -1)
    {
        ObjectInMap *pCreatedObject = NULL;
        int idx = m_pIdxMgr->buildingIndex();

        if(idx == -1)
        {
            printf("%s <- Index Full\n", __FUNCTION__);
            return false;
        }
    
        pObj->SetIndex(idx);
        
        if( (pCreatedObject = m_pMap->addObject(pObj, m_pInfoMgr, 0)) )
            m_pIdxMgr->addBuildingIndex(idx);
        else return false;
        
        if(_newObject(m_pPlayer->GetUserID(), pObj->GetID(), pObj->GetIndex(), pObj->GetPosition(), pObj->GetDirection()) == false)
        {
            //서버에서 실패한거니까, 서버 통해서 재거하지 말고 클라 자체에서 제거하면되.
            m_pMap->removeObject(pObj->GetIndex());
            m_pIdxMgr->removeBuildIndex(idx);
            return false;
        }
        
        return true;
    }
    
    pObj->SetIndex(index);
    
    if(m_pMap->addObject(pObj, m_pInfoMgr, time))
    {
        m_pIdxMgr->addBuildingIndex(index);
        return true;
    }
    
    return false;
}

bool GameSystem::moveObject(POINT<int> &pos, ObjectInMap *obj2, OBJECT_DIRECTION dir)
{
    POINT<int> tmpPos = obj2->GetPosition();
    OBJECT_DIRECTION tmpDir = obj2->GetDirection();
    
    if(m_pMap->moveObject(pos, obj2, dir) == false)
        return false;
    
    const char *baseURL = "http://swmaestros-sng.appspot.com/buildinglchange?id=%s&index=%d&location=%03d%03d&direction=%s";
    char url[256];
    string direction;
    
    if(dir == OBJECT_DIRECTION_LEFT)
        direction = "false";
    else direction = "true";
    
    sprintf(url, baseURL, m_pPlayer->GetUserID(), obj2->GetIndex(), obj2->GetPosition().x, obj2->GetPosition().y, direction.data());
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error Moveojet\n", __FUNCTION__);
        m_pMap->moveObject(tmpPos, obj2, tmpDir);
        
        return false;
    }
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    if(_networkNormalResult(&xmlDoc) == false)
    {
        m_pMap->moveObject(tmpPos, obj2, tmpDir);
        return false;
    }
    
    return true;
}

bool GameSystem::addCrop(Field *pField, int id, int time, int index)
{
    if(index == -1)
    {
        int idx = m_pIdxMgr->cropIndex();
        if(idx == -1)
        {
            printf("%s <- Error Crop Index Full\n", __FUNCTION__);
            return false;
        }
        
        if (pField->addCrop(id, 0, idx, m_pInfoMgr))
            m_pIdxMgr->addCropIndex(idx);
        else return false;
        
        if (_newObject(m_pPlayer->GetUserID(), id, idx, pField->GetPosition(), pField->GetDirection()) == false) {
            m_pIdxMgr->removeCropIndex(idx);
            m_pMap->removeCrop(pField);
        }
        
        return true;
    }
    
    if(m_pMap->addCrop(pField, id, time, index, m_pInfoMgr))
    {
        m_pIdxMgr->addCropIndex(index);
        return true;
    }
    
    return false;
}

bool GameSystem::_removeNetworkObject(const char *userID, int index)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/vbdelete?id=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, userID, index);

    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    return _networkNormalResult(&xmlDoc);
}

void GameSystem::removeCrop(Field *pField)
{
    if(pField->GetCrop() == NULL) return;
    int index = pField->GetCrop()->GetIndex();
    if(_removeNetworkObject(m_pPlayer->GetUserID(), index))
    {
        m_pMap->removeCrop(pField);
        m_pIdxMgr->removeCropIndex(index);
    }
}

void GameSystem::removeObject(POINT<int> &pos)
{
    ObjectInMap *pObj = m_pMap->FindObject(pos);
    int index = pObj->GetIndex();
    
    if(_removeNetworkObject(m_pPlayer->GetUserID(), index))
    {
        m_pMap->removeObject(pObj);
        m_pIdxMgr->removeBuildIndex(index);
    }
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

vector< pair<ObjectInMap, long long int> > GameSystem::_parseObjectInVillage(const char* pContent)
{
    vector< pair<ObjectInMap, long long int> > v;
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(const_cast<char*>(pContent));
    
    printf("%s\n", pContent);
    
    xml_node<char> *pRoot = xmlDoc.first_node()->first_node();
    
    int count = atoi(pRoot->value());
    pRoot = pRoot->next_sibling();
    
    DateInfo serverDate;
    if( _getServerTime(&serverDate) == false )
    {
        printf("%s <- Error\n", __FUNCTION__);
        return v;
    }

    for(int i=0; i<count; ++i, pRoot = pRoot->next_sibling())
    {
        xml_node<char> *pNode = pRoot->first_node();
        
        int index = atoi(pNode->value());
        pNode = pNode->next_sibling();
        int id = atoi(pNode->value());
        pNode = pNode->next_sibling();

        OBJECT_TYPE type;
        objectState state = atoi(pNode->value());
        SIZE<int> size;
        
        if( index >= 1000 )
        {
            type = OBJECT_TYPE_CROP;

            if(state == NETWORK_OBJECT_DONE)
                state = CROP_STATE_DONE;
            else if( state == NETWORK_OBJECT_FAIL)
                state = CROP_STATE_FAIL;
            else state = CROP_STATE_GROW_1;
        }
        else
        {
            if( id != -1 )
            {
                type = OBJECT_TYPE_BUILDING;
            
                switch (state) {
                    case NETWORK_OBJECT_CONSTRUCTION:   state = BUILDING_STATE_UNDER_CONSTRUCTION_1;    break;
                    case NETWORK_OBJECT_WORKING:        state = BUILDING_STATE_WORKING;                 break;
                    case NETWORK_OBJECT_DONE:           state = BUILDING_STATE_DONE;                    break;
                    case NETWORK_OBJECT_OTHER_WATTING:  state = BUILDING_STATE_OTEHR_WORKING;           break;
                    default:break;
                }

                BuildingInfo *pInfo;
                m_pInfoMgr->searchInfo(id, &pInfo);
                size = pInfo->GetSize();
            }
            else
            {
                type = OBJECT_TYPE_FIELD;
                state = 0;
                size = SIZE<int>(1,1);
            }
        }
        pNode = pNode->next_sibling();
        
        int posValue = atoi(pNode->value());
        POINT<int> pos;
        pos.x = GETWORD_X(posValue);
        pos.y = GETWORD_Y(posValue);
        pNode = pNode->next_sibling();
        
        OBJECT_DIRECTION dir = static_cast<OBJECT_DIRECTION>(atoi(pNode->value()));
        pNode = pNode->next_sibling();
        
        const char *pDate = pNode->value();
        DateInfo date;
        date.UpdateDate(pDate);

        long long int deltaTime = date.GetTimeValue(serverDate);
                
        ObjectInMap obj = ObjectInMap(state, pos, size, dir, id, index);
        obj.SetType(type);
        
        pair<ObjectInMap, long long int> value(obj, deltaTime);
        v.push_back(value);
    }
        
    return v;
}

bool GameSystem::_getServerTime(DateInfo *pInfo)
{
    CURL_DATA data;
    if(m_pNetwork->connectHttp("http://swmaestros-sng.appspot.com/timeprint", &data) != CURLE_OK)
    {
        printf("%s <- Error Get Server Time\n", __FUNCTION__);
        return false;
    }
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pNode = xmlDoc.first_node()->first_node();
    pInfo->UpdateDate(pNode->value());
    
    return true;
}

bool GameSystem::SetUpVillageList(bool isUpdate)
{
    const char *baseURL;
    
    if(isUpdate)baseURL = "http://swmaestros-sng.appspot.com/vbstateupdate?id=%s";
    else        baseURL = "http://swmaestros-sng.appspot.com/vbinfolist?id=%s";
    
    char url[256];
    
    sprintf(url, baseURL, m_pPlayer->GetUserID());
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    vector< pair<ObjectInMap, long long int> > v = _parseObjectInVillage(data.pContent);
    vector< pair<ObjectInMap, long long int> > vCrop;
    
    vector< pair<ObjectInMap, long long int> >::iterator iter;
    
    long long time;
    ObjectInMap *pObj;
    
    for(iter = v.begin(); iter != v.end(); ++iter)
    {
        time = (*iter).second;
        pObj = &((*iter).first);
        
        if(pObj->GetType() != OBJECT_TYPE_CROP)
            addObject(pObj, time, pObj->GetIndex());
        
        else vCrop.push_back((*iter));
    }
    
    for(iter = vCrop.begin(); iter != vCrop.end(); ++iter)
    {
        time = (*iter).second;
        pObj = &(*iter).first;
        
        Field *pField = dynamic_cast<Field*>(m_pMap->FindObject(pObj->GetPosition()));
        addCrop(pField, pObj->GetID(), time, pObj->GetIndex());
    }        
    
    return true;
}

void GameSystem::removeObject(ObjectInMap *pObj)
{
    m_pMap->removeObject(pObj);
}