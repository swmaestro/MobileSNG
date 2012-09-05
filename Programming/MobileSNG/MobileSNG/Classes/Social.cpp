//
//  Social.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 25..
//
//

#include "Social.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;

Social::Social(Network *pNetwork, Player *pMe)
{
    m_pNetwork = pNetwork;
    
    m_pPlayer = pMe;
    m_vUsers.push(pMe);
//    m_vUsers.top()
    m_vUsers.top()->GetUserID();
}

Social::~Social()
{
    while (m_vUsers.size() != 0)
    {
        delete m_vUsers.top();
        m_vUsers.pop();
    }
}

void Social::Push(UserInfo *pUserInfo)
{
    User *pUser = new User(pUserInfo->userID.data(), pUserInfo->userPhone.data(), pUserInfo->userDate.data());
    m_vUsers.push(pUser);
}

void Social::Pop()
{
    if(m_vUsers.size() == 1)
        return;
    
    delete m_vUsers.top();
    
    m_vUsers.pop();
}

bool Social::FindUser(const char* p, UserInfo *pOut, USER_SEARCH_ENUM e)
{
    const char *baseURL;// = "http://swmaestros-sng.appspot.com/searchmember?id=%s";
    
    if( e == USER_SEARCH_ENUM_ID )
        baseURL = "http://swmaestros-sng.appspot.com/searchmember?id=%s";
    else
        baseURL = "http://swmaestros-sng.appspot.com/searchnumber?phone=%s";
    
    char url[256];
    sprintf(url, baseURL, p);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    xml_document<char> doc;
    doc.parse<0>(data.pContent);
    
    xml_node<char> *pNode = doc.first_node()->first_node();
    
    if( atoi(pNode->value()) == 0 )
        return false;
    
    pNode = pNode->next_sibling()->first_node();
    
    if(pOut == NULL) return true;

    pOut->userID = pNode->value();
    pNode = pNode->next_sibling();
    pOut->userPhone = pNode->value();
    pNode = pNode->next_sibling();
    pOut->userDate = pNode->value();
//    pNode = pNode->next_sibling();
    
    return true;
}

bool Social::FindVillageInfo(const char* userID, VillageInfo *pOut)
{
    const char* baseURL = "http://swmaestros-sng.appspot.com/villageinfo?id=%s";
    char url[256];
    sprintf(url, baseURL, userID);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    if(pOut == NULL) return true;
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pRoot = xmlDoc.first_node()->first_node();
    
    if( atoi(pRoot->value()) == 0) return false;
    
    pRoot = pRoot->next_sibling();
    
    xml_node<char> *pNode = pRoot->first_node();
    
    pOut->userID = pNode->value();
    pNode = pNode->next_sibling();
    pOut->money = atoi(pNode->value());
    pNode = pNode->next_sibling();
    pOut->cash = atoi(pNode->value());
    pNode = pNode->next_sibling();
    pOut->level = atoi(pNode->value());
    pNode = pNode->next_sibling();
    pOut->exp = atoi(pNode->value());

    return true;
}

bool Social::FindUser(const char* p, VillageInfo *pOut, USER_SEARCH_ENUM e)
{
    UserInfo user;
    if(FindUser(p, &user, e) == false)
    {
        printf("%s <- Error\n", __FUNCTION__);
        return false;
    }
    
    return FindVillageInfo(user.userID.data(), pOut);;
}

std::vector<VillageInfo*> Social::CurrentUserFollowing(int page)
{
    return m_vUsers.top()->GetFollowing(m_pNetwork, page);
}

std::vector<VillageInfo*> Social::CurrentUserFollowers(int page)
{
    return m_vUsers.top()->GetFollowers(m_pNetwork, page);
}

std::vector<VillageInfo*> Social::GetFollowing(const char *userID, int page)
{
    return m_vUsers.top()->GetFollowing(m_pNetwork, page, userID);
}

std::vector<VillageInfo*> Social::GetFollowers(const char *userID, int page)
{
    return m_vUsers.top()->GetFollowers(m_pNetwork, page, userID);
}