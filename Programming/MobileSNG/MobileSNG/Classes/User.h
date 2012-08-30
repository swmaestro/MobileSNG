//
//  People.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 26..
//
//

#pragma once

#include "VillageInfo.h"
#include <vector>
#include <string>
#include "Network.h"

enum FRIEND_GET_ENUM {
    FRIEND_ENUM_FOLLOWING = 1,
    FRIEND_ENUM_FOLLOWERS
    };

class User
{
protected:
    UserInfo                      *m_pUserInfo;
    std::vector<VillageInfo*>      m_vFollowers;
    std::vector<VillageInfo*>      m_vFollowing;
    
public:
    User();
    User(const char* userID, const char* userPhone, const char* userDate);
    ~User();
    
private:
    static std::vector<VillageInfo*> _GetFriend(Network *pNet, int page, const char *userID, FRIEND_GET_ENUM type);

protected:
    void setUserInfo(UserInfo user);

public:
    std::vector<VillageInfo*> GetFollowing(Network *pNet, int page, const char *userID = NULL);
    std::vector<VillageInfo*> GetFollowers(Network *pNet, int page, const char *userID = NULL);

public:
    const char* GetUserID();
};