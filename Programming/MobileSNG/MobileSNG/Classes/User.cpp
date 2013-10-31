//
//  User.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 26..
//
//

#include "User.h"
#include "rapidxml.hpp"
#include "Utility.h"

using namespace std;
using namespace rapidxml;

User::User()
{
    m_pUserInfo = new UserInfo;
}

User::User(const char* userID, const char* userPhone, const char* userDate) : m_pUserInfo(NULL)
{
    m_pUserInfo = new UserInfo(userID, userPhone, userDate);
}

User::~User()
{
    SAFE_DELETE(m_pUserInfo);
    
    std::vector<VillageInfo*>::iterator iter;
    
    for(iter = m_vFollowing.begin(); iter != m_vFollowing.end(); ++iter)
        SAFE_DELETE((*iter));
    for(iter = m_vFollowers.begin(); iter != m_vFollowers.end(); ++iter)
        SAFE_DELETE((*iter));
}

std::vector<VillageInfo*> User::_GetFriend(Network *pNet, int page, const char *userID,FRIEND_GET_ENUM type)
{
    page += 1;
    std::vector<VillageInfo*> vFriend;

    if(pNet == NULL)                    return vFriend;
    
    const char *baseURL = "http://swmaestros-sng.appspot.com/friendinfo?id=%s&type=%d&page=%d";
    char url[256];
    sprintf(url, baseURL, userID, type, page);
    
    CURL_DATA data;
    if(pNet->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error \n", __FUNCTION__);
        return vFriend;
    }
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pRoot = xmlDoc.first_node()->first_node();
    
    int count = atoi(pRoot->value());
    
    pRoot = pRoot->next_sibling();
    
    for(int i=0; i<count; ++i, pRoot = pRoot->next_sibling())
    {
        xml_node<char> *pNode = pRoot->first_node();
        VillageInfo *pVillage = new VillageInfo;
        
        pVillage->userID = pNode->value();
        pNode = pNode->next_sibling();
        pVillage->money = atoi(pNode->value());
        pNode = pNode->next_sibling();
        pVillage->cash = atoi(pNode->value());
        pNode = pNode->next_sibling();
        pVillage->level = atoi(pNode->value());
        pNode = pNode->next_sibling();
        pVillage->exp = atoi(pNode->value());
        
        vFriend.push_back(pVillage);
    }
    
    return vFriend;
}
 
std::vector<VillageInfo*> User::GetFollowing(Network *pNet, int page, const char *userID)
{
    if(userID == NULL) userID = m_pUserInfo->userID.data();
    return _GetFriend(pNet, page, userID, FRIEND_ENUM_FOLLOWING);
}

std::vector<VillageInfo*> User::GetFollowers(Network *pNet, int page, const char *userID)
{
    if(userID == NULL) userID = m_pUserInfo->userID.data();
    return _GetFriend(pNet, page, userID, FRIEND_ENUM_FOLLOWERS);
}

void User::setUserInfo(UserInfo user)
{
    *m_pUserInfo = user;
}

const char* User::GetUserID()
{
    return m_pUserInfo->userID.data();
}