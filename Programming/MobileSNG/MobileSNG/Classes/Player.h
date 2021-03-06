//
//  Player.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 13..
//
//

#pragma once

#include <string>
#include "Sqlite3Base.h"
#include "Network.h"
#include "VillageInfo.h"
#include "User.h"
#include "ObjectInMap.h"

#define PLAYER_FILE_NAME "Player.info"

enum FRIEND_ENUM
{
    FRIEND_ENUM_ID,
    FRIEND_ENUM_PHONE
};

struct REQUEST {
    std::string     requester;
    int             id;
    
    REQUEST():id(0) {}
};

class Player : public User
{
private:
    std::string     m_strFilePath;
    std::string     m_strPassWord;
    VillageInfo     *m_pVillageInfo;
    
public:
    Player(Network *pNetwork);
    ~Player();

public:
    bool AddMoney(int n);
    void AddCash(int n);
    void AddExp(int n);
    
public:
    int GetLevel();
    int GetMoney();
    int GetExp();
    int GetMinumExp();
    bool UpdateVillageInfo(Network *pNet);
    
public:
    bool addFollowing(const char * userID, Network *pNet);
    bool removeFollowing(User *pUser, Network *pNet);
    std::vector<REQUEST> viewRequestList(Network *pNet, int page, bool isMeRequest);
    
public:
    static bool hasFile();
    static void newPlayer(const char *PlayerID, const char *PlayerPW, const char *PlayerPhone, char *PlayerDate);
    static void GetInfo(char *pOutID, char *pOutPW, char *pOutPhone, char *pOutDate);
};