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
#include "CommonVillage.h"
#include <vector>

class FriendVillage : public CommonVillage
{
private:
    MapMgr      *m_pMap;
    UserInfo    *m_pUserInfo;
    int         m_nObjectLoop;
    const char* m_playerName;

    bool        m_isWork;
    
public:
    FriendVillage(int mapLevel, UserInfo *pUser, Network *pNetwork, const char* playerName);
    ~FriendVillage();
    
private:
    bool _initMap();
    bool _checkPosiibleWork();
    
public:
    bool init();
    bool Harvest(POINT<int> &pos, ObjectInMap **ppOut);
    
public:
    std::vector<ObjectInMap*>& GetAllObject();
    inline bool GetisPossibleWork() { return m_isWork; }
};