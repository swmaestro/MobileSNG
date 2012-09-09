//
//  UserInfo.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 25..
//
//

#pragma once

#include <string>
#include "UserInfo.h"

class Network;

class VillageInfo
{
public:
    std::string     userID;
    int             level;
    int             money;
    int             cash;
    int             exp;
    
public:
    VillageInfo();
    VillageInfo(const char *userID, int level, int money, int cash, int exp);
    ~VillageInfo();
    
public:
    bool UpdateVillageInfo(Network *pNetwork);
    
public:
    int GetMaximumExp();
};

