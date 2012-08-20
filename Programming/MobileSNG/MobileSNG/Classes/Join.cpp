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

Join::Join(Network *pNetwork, JoinUI *pUI)
{
    m_pNetwork  = pNetwork;
    m_pUI       = pUI;
}

Join::Join()
{
    
}

bool Join::CreatAccount()
{
    const char *strID       = m_pUI->GetContext(JOIN_UI_ENUM_ID);
    const char *strPW       = m_pUI->GetContext(JOIN_UI_ENUM_PW);
    const char *strPhone    = m_pUI->GetContext(JOIN_UI_ENUM_PHONE);
    
    return _Join(strID, strPW, strPhone);
}

bool Join::Overlab()
{
    const char *strID = m_pUI->GetContext(JOIN_UI_ENUM_ID);
    return _CheckOverlapID(strID);
}

bool Join::_CheckPhoneNumber(const char *strPhone)
{
    int num = strlen(strPhone);
    
    if( (num == 10 || num == 11) == false )
        return false;
    
    int size = strlen(strPhone);
    
    for(int i=0; i<size; ++i)
        if( ('0' <= strPhone[i] && strPhone[i] < '9') == false )
            return false;
    
    return true;
}

bool Join::_Join(const char *strID, const char *strPassword, const char *strPhone)
{
    const char *baseUrl = "http://swmaestros-sng.appspot.com/sngtestmodule?id=%s&password=%s&phone=%s";

    char url[256];
    
    if(_CheckPhoneNumber(strPhone) == false)    return false;
    
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

bool Join::_CheckOverlapID(const char *strID)
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