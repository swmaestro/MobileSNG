//
//  FriendVillage.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 31..
//
//

#include "FriendVillage.h"

using namespace std;
using namespace rapidxml;

FriendVillage::FriendVillage(int mapLevel, UserInfo *pUser, Network *pNetwork, const char* playerName) : CommonVillage(pNetwork), m_isWork(false)
{
    m_pInfoMgr->loadData("ObjectDB.sqlite");
    m_pMap  = new MapMgr(mapLevel);
    m_pUserInfo = new UserInfo(pUser->userID.data(), pUser->userPhone.data(), pUser->userDate.data());
    m_nObjectLoop = 0;
    m_playerName = playerName;
}

FriendVillage::~FriendVillage()
{
    SAFE_DELETE(m_pMap);
    SAFE_DELETE(m_pUserInfo);
}

bool FriendVillage::_initMap()
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/vbstateupdate?id=%s";
    char url[256];
    sprintf(url, baseURL, m_pUserInfo->userID.data());
    printf("%s\n", url);
    
    CURL_DATA buildingData;
    if(m_pNetwork->connectHttp(url, &buildingData) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    baseURL = "http://swmaestros-sng.appspot.com/crop_rlist?id=%s";
    sprintf(url, baseURL, m_pUserInfo->userID.data());
    printf("%s\n", url);
    
    CURL_DATA cropData;
    if(m_pNetwork->connectHttp(url, &cropData) != CURLE_OK)
        return false;
    
    vector< pair<ObjectInMap, double> > vBuild = _parseBuildingInVillage(buildingData.pContent);
    vector< pair<int, int> > vCrop = _parseCropInVillage(cropData.pContent);
    vector< pair<int, int> > vFieldTime;
    
    for(vector< pair<ObjectInMap, double> >::iterator
        iter = vBuild.begin(); iter != vBuild.end(); ++iter)
    {
        //밭과 건물을 비교하는 방법. index비교.
        ObjectInMap *pObject = &(*iter).first;
        double time = (*iter).second;
        
        m_pMap->addObject(pObject, m_pInfoMgr, time);
        
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
//        addCrop(pField, cropID, _findFieldTime(pField->GetIndex()));
        
        m_pMap->addCrop(pField, cropID, 0, m_pInfoMgr);
    }
    
    return true;
}

bool FriendVillage::_checkPosiibleWork()
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/isrequest?owner=%s&requester=%s";
    char url[256];
    sprintf(url, baseURL, m_pUserInfo->userID.data(), m_playerName);

    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    if( _networkNormalResult(&data) == false)
        return false;

    return true;
}

bool FriendVillage::init()
{
    if(_initMap() == false)
        return false;
    
    m_isWork = _checkPosiibleWork();
    
    return true;
}

bool FriendVillage::_request(int index)
{
    const char *baseURL ="http://swmaestros-sng.appspot.com/brequest_create?owner=%s&requester=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_pUserInfo->userID.data(), m_playerName, index);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    return _networkNormalResult(&data);
}

bool FriendVillage::_ownerCheckRequest(int index)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/brequest_accept?owner=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_pUserInfo->userID.data(), index);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    return _networkNormalResult(&data);
}

bool FriendVillage::Request(POINT<int> &pos, ObjectInMap **ppOut)
{    
    ObjectInMap *pObj = m_pMap->FindObject(pos);

    if(pObj == NULL)                                return false;
    if(pObj->GetType()  != OBJECT_TYPE_BUILDING)    return false;
    if(pObj->GetState() != BUILDING_STATE_WORKING)  return false;
    
    int index = pObj->GetIndex();

    if(_request(index) == false)            return false;
    if(_ownerCheckRequest(index) == false)  return false;
    
    return true;
}

ObjectInMap* FriendVillage::FindObject(POINT<int> pos)
{
    return m_pMap->FindObject(pos);
}

std::vector<ObjectInMap*> FriendVillage::FindObjects(POINT<int> pos, SIZE<int> size)
{
    return m_pMap->FindObjects(pos, size);
}

std::vector<ObjectInMap*>& FriendVillage::GetAllObject()
{
    return m_pMap->GetAllObject();
}
