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
#include "Network.h"

#define USER_FILE_NAME "User.info"

class User
{
private:
    std::string     m_strFilePath;
    std::string     m_strID;
    std::string     m_strPassWord;
    std::string     m_strPhoneNumber;

    int             m_level;
    int             m_money;
    int             m_cash;
    int             m_exp;
    
public:
    User();
    ~User();
    
public:
    void SetData(char *xmlData);

public:
    bool AddMoney(int n);
    void AddCash(int n);
    void AddExp(int n);
    
public:
    int     GetLevel();
    int     GetMoney();
    int     GetCash();
    int     GetExp();
    int     GetMaximum();
    
public:
    static bool hasFile();
    static void newUser(const char *userID, const char *userPW, const char *userPhone);
    static void GetInfo(char *pOutID, char *pOutPW, char *pOutPhone);
};