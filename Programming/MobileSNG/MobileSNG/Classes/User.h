//
//  User.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 13..
//
//

#pragma once

#include <string>
#include "Sqlite3Base.h"
#include "NetWork.h"

#define USER_FILE_NAME "User.info"

class User
{
private:
    std::string     m_strUserFilePath;
    std::string     m_strUserID;
    std::string     m_strPassWord;

    int             m_level;
    int             m_money;
    int             m_cash;
    int             m_exp;
    
public:
    User(const char *FileName = USER_FILE_NAME);
    ~User();
    
public:
    void SetData(char *xmlData);
    
public:
    bool Login(NetWork *pNetWork);
    bool LogOut(NetWork *pNetWork);
    
public:
    void AddMoney(int n);
    void AddCash(int n);
    void AddExp(int n);
    
public:
    bool    isEmpty();
    int     GetLevel();
    int     GetMoney();
    int     GetCash();
    int     GetExp();
};