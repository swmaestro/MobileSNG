//
//  UserInfo.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 25..
//
//

#pragma once

#include <string>

struct UserInfo
{
    std::string     userPhone;
    std::string     userID;
    std::string     userDate;
    
public:
    UserInfo(){}
    UserInfo(const char* userID, const char *userPhone, const char *userDate)
    {
        this->userID        = userID;
        this->userPhone     = userPhone;
        this->userDate      = userDate;
    }
    
    ~UserInfo(){}
};