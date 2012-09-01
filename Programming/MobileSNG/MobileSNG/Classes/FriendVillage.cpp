//
//  FriendVillage.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 31..
//
//

#include "FriendVillage.h"

using namespace std;

FriendVillage::FriendVillage(int mapLevel, UserInfo *pUser, std::vector< std::pair<ObjectInMap, long long int> > vData, ObjectInfoMgr *pInfoMgr)
{
    m_pMap  = new MapMgr(mapLevel);
    m_pUserInfo = new UserInfo(pUser->userID.data(), pUser->userPhone.data(), pUser->userDate.data());
    
    vector< pair<ObjectInMap, long long int> >::iterator iter;
    vector< pair<ObjectInMap, long long int> > vCrop;
    
    ObjectInMap     *pObj;
    int              time;
    
    for(iter = vData.begin(); iter != vData.end(); ++iter)
    {
        pObj = &(*iter).first;
        time = (*iter).second;
        OBJECT_TYPE type = pObj->GetType();
        
        if( type != OBJECT_TYPE_CROP )
            m_pMap->addObject(pObj, pInfoMgr, time);
        else    vCrop.push_back((*iter));
    }
    
    for(iter = vCrop.begin(); iter != vCrop.end(); ++iter)
    {
        pObj = &(*iter).first;
        time = (*iter).second;
        
        Field *pField = dynamic_cast<Field*>(m_pMap->FindObject(pObj->GetIndex()));
        m_pMap->addCrop(pField, pObj->GetID(), time, pObj->GetIndex(), pInfoMgr);
    }
    
    m_objectIter = m_pMap->GetAllObject().begin();
}

FriendVillage::~FriendVillage()
{
    SAFE_DELETE(m_pMap);
    SAFE_DELETE(m_pUserInfo);
}

bool FriendVillage::UpdateMapObject(ObjectInMap **ppOut)
{
    vector<ObjectInMap*>::iterator begin = m_pMap->GetAllObject().begin();
    vector<ObjectInMap*>::iterator end   = m_pMap->GetAllObject().end();
    
    if( m_pMap->GetAllObject().size() == 0 )
        return false;
    
    if(m_objectIter == end)
        m_objectIter++ = begin;
    
    *ppOut = *m_objectIter;
    
    return (*m_objectIter)->UpdateSystem();
}

MapMgr* FriendVillage::GetMapMgr()
{
    return m_pMap;
}

