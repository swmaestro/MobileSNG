//
//  VillageInfo.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 9. 9..
//
//

#include "VillageInfo.h"
#include "Network.h"
#include "rapidxml.hpp"

using namespace rapidxml;

VillageInfo::VillageInfo() : level(0), money(0), cash(0), exp(0)
{
    
}

VillageInfo::VillageInfo(const char *userID, int level, int money, int cash, int exp)
{
    this->userID = userID;
    this->level = level;
    this->money = money;
    this->cash = cash;
    this->exp = exp;
}

VillageInfo::~VillageInfo()
{
    
}

int VillageInfo::GetMaximumExp()
{
    return level * 200;
}

bool VillageInfo::UpdateVillageInfo(Network *pNetwork)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/villageinfo?id=%s";
    char url[256];
    sprintf(url, baseURL, userID.data());
    
    CURL_DATA data;
    if(pNetwork->connectHttp(url, &data) != CURLE_OK )
    {
        printf("%s <- Player Update Error", __FUNCTION__);
        return false;
    }
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pNode = xmlDoc.first_node()->first_node()->next_sibling()->first_node()->next_sibling();
    
    money = atoi(pNode->value());
    pNode = pNode->next_sibling();
    cash  = atoi(pNode->value());
    pNode = pNode->next_sibling();
    level = atoi(pNode->value());
    pNode = pNode->next_sibling();
    exp = (int)atof(pNode->value());

    return true;
}