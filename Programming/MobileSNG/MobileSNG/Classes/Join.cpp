//
//  Join.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 15..
//
//

#include "Join.h"
#include "rapidxml.hpp"
#include "JoinUI.h"

using namespace cocos2d;
using namespace rapidxml;

Join::Join(Network *pNetwork)
{
    m_pNetwork  = pNetwork;
}

Join::Join()
{
    
}

bool Join::_CreateVillage(const char *userID, const int initGold, const int initCash)
{
    const char *baseUrl = "http://swmaestros-sng.appspot.com/villageregister?id=%s&costA=%d&costB=%d";
    char url[256];
    
    sprintf(url, baseUrl, userID, initGold, initCash);
    
    CURL_DATA data;
    if(m_pNetwork->connectHttp(url, &data) != CURLE_OK)
        return false;
    
    return true;
}

bool Join::_CheckPhoneNumber(const char *strPhone)
{
    int num = strlen(strPhone);
    
    if( (num == 10 || num == 11) == false )
        return false;
    
    int size = strlen(strPhone);
    
    for(int i=0; i<size; ++i)
        if( ('0' <= strPhone[i] && strPhone[i] <= '9') == false )
            return false;
    
    return true;
}

bool Join::CreatAccount(const char *strID, const char *strPassword, const char *strPhone)
{
    const char *baseUrl = "http://swmaestros-sng.appspot.com/sngtestmodule?id=%s&password=%s&phone=%s";

    char url[256];
    
    if(_CheckPhoneNumber(strPhone) == false)
        return false;
    
    sprintf(url, baseUrl, strID,strPassword, strPhone);
    
    CURL_DATA data;
    
    if( m_pNetwork->connectHttp(url, &data) != CURLE_OK )
    {
        printf("%s <- Join Fail / Not Connect \n", __FUNCTION__);
        return false;
    }
    
    printf("%s\n",data.pContent); // 파싱 해야지
    
    rapidxml::xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);

    xml_node<char> *node = xmlDoc.first_node()->first_node();//->next_sibling();

    if( strcmp("false",node->value()) == 0)
    {
        printf("%s <- Join Fail", __FUNCTION__);
        return false;
    }
    
    return true;
}

bool Join::CheckOverlapID(const char *strID)
{
    const char *baseUrl = "http://swmaestros-sng.appspot.com/signupnametest?id=%s";
    
    char url[256];
    sprintf(url, baseUrl, strID);
    
    CURL_DATA data;
    
    if( m_pNetwork->connectHttp(url, &data) != CURLE_OK )
    {
        printf("%s <- CheckOverlapID Fail / Not Connect \n", __FUNCTION__);
        return false;
    }
    
    printf("%s\n",data.pContent); // 파싱 해야지

    rapidxml::xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *node = xmlDoc.first_node()->first_node();
    
    if( strcmp("true",node->value()) == 0)
        return true;
    
    return false;
}