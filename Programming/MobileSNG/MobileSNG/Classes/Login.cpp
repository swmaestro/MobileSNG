//
//  Login.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 21..
//
//

#include "Login.h"
#include "rapidxml.hpp"

using namespace rapidxml;

Login::Login(Network *pNetwork)
{
    m_pNetwork = pNetwork;
}

Login::~Login()
{
    
}

bool Login::Logon(const char *userID, const char *userPW)
{
    static const char *baseUrl = "http://swmaestros-sng.appspot.com/memberlogin?id=%s&password=%s";
    
    char url[256];
    sprintf(url, baseUrl, userID, userPW);

    CURL_DATA data;
    
    if( m_pNetwork->connectHttp(url, &data) != CURLE_OK )
    {
        printf("%s <- Login Fail/ Not Connect \n", __FUNCTION__);
        return false;
    }
    
    printf("%s\n", data.pContent);
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pNode = xmlDoc.first_node()->first_node();
    
    if( strcmp("false", pNode->value()) == 0)
    {
        printf("%s <- Join Fail", __FUNCTION__);
        return false;
    }
    
    return true;
}