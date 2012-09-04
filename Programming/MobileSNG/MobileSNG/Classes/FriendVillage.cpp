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
            m_pMap->addObject(pObj, m_pInfoMgr, time);
        
        else vCrop.push_back((*iter));
    }
    
    for(iter = vCrop.begin(); iter != vCrop.end(); ++iter)
    {
        time = (*iter).second;
        pObj = &(*iter).first;
        
        Field *pField = dynamic_cast<Field*>(m_pMap->FindObject(pObj->GetPosition()));
        m_pMap->addCrop(pField, pObj->GetID(), time, pObj->GetIndex(), m_pInfoMgr);
    }
    
    return true;
}

bool FriendVillage::_checkPosiibleWork()
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/isrequest?owner=%s&requester=%s";
    char url[256];
    sprintf(url, baseURL, m_playerName, m_pUserInfo->userID.data());
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    rapidxml::xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    if( _networkNormalResult(&xmlDoc) == false)
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

bool FriendVillage::Harvest(POINT<int> &pos, ObjectInMap **ppOut)
{    
    ObjectInMap *pObj = m_pMap->FindObject(pos);

    if(pObj == NULL)                                return false;
    if(pObj->GetType()  != OBJECT_TYPE_BUILDING)    return false;
    if(pObj->GetState() != BUILDING_STATE_WORKING)  return false;
    
    int index = pObj->GetIndex();
    
    const char *baseURL ="http://swmaestros-sng.appspot.com/brequest_create?owner=%s&requester=%s&index=%d";
    char url[256];
    sprintf(url, baseURL, m_playerName, m_pUserInfo->userID.data(), index);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
        
    return _networkNormalResult(&xmlDoc);
}

std::vector<ObjectInMap*>& FriendVillage::GetAllObject()
{
    return m_pMap->GetAllObject();
}
