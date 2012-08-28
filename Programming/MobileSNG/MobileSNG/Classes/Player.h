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

#define PLAYER_FILE_NAME "Player.info"

enum FRIEND_ENUM
{
    FRIEND_ENUM_ID,
    FRIEND_ENUM_PHONE
};

class Player : private User
{
private:
    std::string     m_strFilePath;
    std::string     m_strPassWord;
    VillageInfo  *m_pVillageInfo;
    
public:
    Player();
    ~Player();
    
public:
    void UpdateData(Network *pNetwork);

public:
    bool AddMoney(int n);
    void AddCash(int n);
    void AddExp(int n);
    
public:
    int GetLevel();
    int GetMoney();
    int GetExp();
    int GetMinumExp();
    
public:
    bool addFollowing(User *pUser, Network *pNet);
    bool removeFollowing(User *pUser, Network *pNet);
    
public:
    static bool hasFile();
    static void newPlayer(const char *PlayerID, const char *PlayerPW, const char *PlayerPhone, char *PlayerDate);
    static void GetInfo(char *pOutID, char *pOutPW, char *pOutPhone, char *pOutDate);
};