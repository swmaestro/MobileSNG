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
    m_isDone    = false;
}

Join::Join()
{
}

void Join::_btJoin(cocos2d::CCObject *pSender)
{
    JoinUI *pUI = static_cast<JoinUI*>(pSender);
    
    const char *strID       = pUI->GetContext(JOIN_UI_ENUM_ID);
    const char *strPW       = pUI->GetContext(JOIN_UI_ENUM_PW);
    const char *strPhone    = pUI->GetContext(JOIN_UI_ENUM_PHONE);
    
    m_isDone = _Join(strID, strPW, strPhone);
    if( m_isDone == false)
    {
        CCMessageBox("Join Fail", "Error");
        for(int i=0; i<JOIN_UI_ENUM_NUM; ++i)
            m_pUI->setEmptyTextField((JOIN_UI_ENUM)i);
    }
}

void Join::_btCancel(cocos2d::CCObject *pSender)
{
    m_isDone = true;
}

void Join::_btOverlab(cocos2d::CCObject *pSender)
{
    JoinUI *pUI = static_cast<JoinUI*>(pSender);
    const char *strID = pUI->GetContext(JOIN_UI_ENUM_ID);

    if(_CheckOverlapID(strID))    CCMessageBox("Overlab ID", "Error");
    else                         CCMessageBox("OK", "OK");
}

bool Join::_CheckPhoneNumber(const char *strPhone)
{
    //이따가 생각. 중요한건 아냐
    int num = strlen(strPhone);
    
    if( num == 10 || num == 11 )
        return true;
    
    return false;
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

bool Join::isDone()
{
    return m_isDone;
}