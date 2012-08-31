//
//  FriendVillage.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 31..
//
//

#pragma once

#include "User.h"
#include "MapMgr.h"
#include <vector>

class FriendVillage
{
private:
    MapMgr      *m_pMap;
    UserInfo    *m_pUserInfo;
    std::vector<ObjectInMap*>::iterator m_objectIter;
    
    
public:
    FriendVillage(int mapLevel, UserInfo *pUser, std::vector< std::pair<ObjectInMap, long long int> > vData, ObjectInfoMgr *pInfoMgr);
    ~FriendVillage();
    
public:
    bool UpdateMapObject(ObjectInMap **ppOut);
    MapMgr* GetMapMgr();
};