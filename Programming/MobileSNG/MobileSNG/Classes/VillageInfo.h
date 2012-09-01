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

struct VillageInfo
{
    std::string     userID;
    int             level;
    int             money;
    int             cash;
    int             exp;
    
public:
    VillageInfo() : level(0), money(0), cash(0), exp(0) {}
    VillageInfo(const char *userID, int level, int money, int cash, int exp)
    {
        this->userID = userID;
        this->level = level;
        this->money = money;
        this->cash = cash;
        this->exp = exp;
    }
    
    ~VillageInfo(){}
    
    int GetMaximumExp()
    {
        return level * 200;
    }
};

