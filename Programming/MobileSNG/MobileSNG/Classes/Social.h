//
//  Social.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 25..
//
//

#pragma once

#include "Network.h"
#include "User.h"
#include "Player.h"
#include <stack>
#include <vector>

enum USER_SEARCH_ENUM
{
    USER_SEARCH_ENUM_ID,
    USER_SEARCH_ENUM_PHONE
};

class Social
{
private:
    Network                 *m_pNetwork;
    std::stack<User*>        m_vUsers;
    Player                  *m_pPlayer;
    
public:
    Social(Network *pNetwork, Player *pMe);
    ~Social();
    
public:
    void Push(UserInfo *pUserInfo);
    void Pop();
    
public:
    bool FindVillageInfo(const char* userID, VillageInfo *pOut);
    
public:
    bool FindUser(const char* p, UserInfo *pOut, USER_SEARCH_ENUM e);
    bool FindUser(const char* p, VillageInfo *pOut, USER_SEARCH_ENUM e);
    
public:
    std::vector<VillageInfo*> CurrentUserFollowing(int page);
    std::vector<VillageInfo*> CurrentUserFollowers(int page);
    
    std::vector<VillageInfo*> GetFollowing(const char *userID, int page);
    std::vector<VillageInfo*> GetFollowers(const char *userID, int page);
};
