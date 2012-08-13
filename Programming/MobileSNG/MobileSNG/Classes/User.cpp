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

using namespace cocos2d;
using namespace std;

User::User(const char *FileName)
{
    m_strUserFilePath = CCFileUtils::sharedFileUtils()->getWriteablePath().append(FileName);
    
    FILE *pFile = fopen(m_strUserFilePath.data(), "rb");
    
    if(pFile != NULL)
    {
        char txt[32];
        
        fscanf(pFile, "%s\n", txt);
        m_strUserID = txt;
        
        fscanf(pFile, "%s\n", txt);
        m_strPassWord = txt;
    }
    
    fclose(pFile);
}

User::~User()
{
    FILE *pFile = fopen(m_strUserFilePath.data(), "wb");
    
    fprintf(pFile, "%s\n",m_strUserID.data());
    fprintf(pFile, "%s\n",m_strUserID.data());
    
    fclose(pFile);
}

void User::SetData(char *xmlData)
{
    if( m_strUserID.empty() == false )
    {
        //이미 있었으니까 파싱할필요 없이 스킵하면 될거같아
    }
    
    //파싱
}

bool User::Login(NetWork *pNetWork)
{
    
    
    return true;
}

bool User::LogOut(NetWork *pNetWork)
{
    
    
    return true;
}

void User::AddMoney(int n)
{
    m_money += n;
}

void User::AddCash(int n)
{
    m_cash += n;
}

void User::AddExp(int n)
{
    //경험치 곡선을 그리든가 공식으로 하든가 해서 현재 레벨의 다음걸 알아서 유추해봐
    const int maxExp = 100;
    
    if( m_exp >= maxExp )
    {
        ++m_level;
        m_exp = 0;
    }
}

bool User::isEmpty()
{
    //id가 없다는건 생성이 안된것이기 때문에 이런식으로 ㅇㅇ.
    return m_strUserID.empty();
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