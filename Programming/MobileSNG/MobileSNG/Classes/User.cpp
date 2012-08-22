//
//  User.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 13..
//
//

#include "User.h"
#include "CCFileUtils.h"
#include <stdlib.h>
#include "Utility.h"

using namespace cocos2d;
using namespace std;

User::User()
{
    m_strFilePath = CCFileUtils::sharedFileUtils()->getWriteablePath().append(USER_FILE_NAME);
    
    FILE *pFile = fopen(m_strFilePath.data(), "rb");
    
    if(pFile != NULL)
    {
        char txt[32];
        
        fscanf(pFile, "%s", txt);
        m_strID = txt;
        
        fscanf(pFile, "%s", txt);
        m_strPassWord = txt;
        
        fscanf(pFile, "%s", txt);
        m_strPhoneNumber = txt;
    }
    
    m_money = 100000;
    m_cash  = 10000;
    m_level = 100;
    m_exp = 0;
    
    fclose(pFile);
}

User::~User()
{
    FILE *pFile = fopen(m_strFilePath.data(), "wb");
    
    fprintf(pFile, "%s\n",m_strID.data());
    fprintf(pFile, "%s\n",m_strPassWord.data());
    fprintf(pFile, "%s\n",m_strPhoneNumber.data());
    
    fclose(pFile);
}

bool User::hasFile()
{
    return isExistFile(CCFileUtils::sharedFileUtils()->getWriteablePath().append(USER_FILE_NAME).data());
}

void User::SetData(char *xmlData)
{
    if( m_strID.empty() == false )
    {
        //이미 있었으니까 파싱할필요 없이 스킵하면 될거같아
    }
    
    //파싱
}

bool User::AddMoney(int n)
{
    if( (m_money + n) < 0 )
        return false;

    m_money += n;
    return true;
}

void User::AddCash(int n)
{
    m_cash += n;
}

void User::AddExp(int n)
{
    if( m_exp >= m_level*2 )
    {
        ++m_level;
        m_exp = 0;
    }
}

int User::GetLevel()
{
    return m_level;
}

int User::GetMoney()
{
    return m_money;
}

int User::GetCash()
{
    return m_cash;
}

int User::GetExp()
{
    return m_exp;
}

int User::GetMaximum()
{
    return m_level * 2;
}

void User::newUser(const char *userID, const char *userPW, const char *userPhone)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWriteablePath().append(USER_FILE_NAME);
    FILE *pFile = fopen(path.data(), "wb");
    
    fprintf(pFile, "%s\n",userID);
    fprintf(pFile, "%s\n",userPW);
    fprintf(pFile, "%s",userPhone);
    
    fclose(pFile);
}

void User::GetInfo(char *pOutID, char *pOutPW, char *pOutPhone)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWriteablePath().append(USER_FILE_NAME);
    FILE *pFile = fopen(path.data(), "rb");
    
    if(pOutID)      fscanf(pFile, "%s", pOutID);
    if(pOutPW)      fscanf(pFile, "%s", pOutPW);
    if(pOutPhone)   fscanf(pFile, "%s", pOutPhone);

    fclose(pFile);
}