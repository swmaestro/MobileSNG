//
//  Player.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 13..
//
//

#include "Player.h"
#include "CCFileUtils.h"
#include <stdlib.h>
#include "Utility.h"
#include "rapidxml.hpp"

using namespace cocos2d;
using namespace std;
using namespace rapidxml;

Player::Player()
{
    m_strFilePath = CCFileUtils::sharedFileUtils()->getWriteablePath().append(PLAYER_FILE_NAME);
    
    FILE *pFile = fopen(m_strFilePath.data(), "rb");

    m_pUserInfo         = new UserInfo;
    m_pVillageInfo      = new VillageInfo;
    
    char id[32], pw[32], ph[11], date[30];
    
    if(pFile != NULL)
    {
        fscanf(pFile, "%s", id);
        fscanf(pFile, "%s", pw);
        fscanf(pFile, "%s", ph);
        fscanf(pFile, "%s", date);
        
        m_strPassWord = pw;
        m_pUserInfo->userPhone = ph;
        m_pUserInfo->userID = id;
        m_pUserInfo->userDate = date;
    }
    
    fclose(pFile);
    
    m_pVillageInfo->userID = id;
}

Player::~Player()
{
    FILE *pFile = fopen(m_strFilePath.data(), "wb");
    
    
    fprintf(pFile, "%s\n",m_pUserInfo->userID.data());
    fprintf(pFile, "%s\n",m_strPassWord.data());
    fprintf(pFile, "%s\n",m_pUserInfo->userPhone.data());
    fprintf(pFile, "%s\n",m_pUserInfo->userDate.data());
    
    fclose(pFile);
    
    SAFE_DELETE(m_pVillageInfo);
}

bool Player::hasFile()
{
    return isExistFile(CCFileUtils::sharedFileUtils()->getWriteablePath().append(PLAYER_FILE_NAME).data());
}

void Player::UpdateData(Network *pNetwork)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/villageinfo?id=%s";
    char url[256];
    sprintf(url, baseURL, m_pUserInfo->userID.data());

    CURL_DATA data;
    if(pNetwork->connectHttp(url, &data) != CURLE_OK )
    {
        printf("%s <- Player Update Error", __FUNCTION__);
        return;
    }
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pNode = xmlDoc.first_node()->first_node()->next_sibling()->first_node()->next_sibling();
    
    m_pVillageInfo->money = atoi(pNode->value());
    pNode = pNode->next_sibling();
    m_pVillageInfo->cash  = atoi(pNode->value());
    pNode = pNode->next_sibling();
    m_pVillageInfo->level = atoi(pNode->value());
    pNode = pNode->next_sibling();
    m_pVillageInfo->exp = (int)atof(pNode->value());
}

bool Player::AddMoney(int n)
{
    if( (m_pVillageInfo->money + n) < 0 )
        return false;

    m_pVillageInfo->money += n;
    return true;
}

void Player::AddCash(int n)
{
    m_pVillageInfo->cash += n;
}

void Player::AddExp(int n)
{
    m_pVillageInfo->exp+=n;
    if( m_pVillageInfo->exp >= m_pVillageInfo->level*200 )
    {
        ++(m_pVillageInfo->level);
        m_pVillageInfo->exp = 0;
    }
}

bool Player::addFollowing(User *pPlayer, Network *pNet)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/friendadder?id=%s&friend=%s";
    char url[256];
    sprintf(url, baseURL, m_pVillageInfo->userID.data(), pPlayer->GetUserID());

    CURL_DATA data;
    if(pNet->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }

    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pNode = xmlDoc.first_node()->first_node();
    
    if(strcmp(pNode->value(), "true") != 0)
        return false;
    
    return true;
}

bool Player::removeFollowing(User *pPlayer, Network *pNet)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/frienddelete?id=%s&friend=%s";
    char url[128];
    sprintf(url, baseURL, m_pUserInfo->userID.data(), pPlayer->GetUserID());
    
    CURL_DATA data;
    if( pNet->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pNode = xmlDoc.first_node()->first_node();
    
    if(strcmp(pNode->value(), "true") != 0)
        return false;
    
    return true;
}

int Player::GetLevel()
{
    return m_pVillageInfo->level;
}

int Player::GetMoney()
{
    return m_pVillageInfo->money;
}

int Player::GetExp()
{
    return m_pVillageInfo->exp;
}

int Player::GetMinumExp()
{
    return m_pVillageInfo->GetMaximumExp();
}

void Player::newPlayer(const char *PlayerID, const char *PlayerPW, const char *PlayerPhone, char *PlayerDate)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWriteablePath().append(PLAYER_FILE_NAME);
    FILE *pFile = fopen(path.data(), "wb");
    
    fprintf(pFile, "%s\n",PlayerID);
    fprintf(pFile, "%s\n",PlayerPW);
    fprintf(pFile, "%s\n",PlayerPhone);
    fprintf(pFile, "%s", PlayerDate);
    
    fclose(pFile);
}

void Player::GetInfo(char *pOutID, char *pOutPW, char *pOutPhone, char *pOutDate)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWriteablePath().append(PLAYER_FILE_NAME);
    FILE *pFile = fopen(path.data(), "rb");
    
    if(pOutID)      fscanf(pFile, "%s", pOutID);
    if(pOutPW)      fscanf(pFile, "%s", pOutPW);
    if(pOutPhone)   fscanf(pFile, "%s", pOutPhone);
    if(pOutDate)    fscanf(pFile, "%s", pOutDate);

    fclose(pFile);
}