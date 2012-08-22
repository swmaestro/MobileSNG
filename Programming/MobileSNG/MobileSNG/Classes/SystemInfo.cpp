//
//  SystemInfo.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 14..
//
//

#include "SystemInfo.h"
#include "rapidxml.hpp"
#include "CCFileUtils.h"
#include <stdlib.h>
#include "Utility.h"

using namespace cocos2d;
using namespace std;

SystemInfo::SystemInfo(Network *pNet) : m_version(0), m_isUpdated(false)
{
    string filePath = CCFileUtils::sharedFileUtils()->getWriteablePath() + SYSTEM_FILE_NAME;
    
    if(isExistFile(filePath.data()) == false)
        filePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(SYSTEM_FILE_NAME);
    
    FILE *pFile = fopen(filePath.data(), "rb");
    if(pFile)fscanf(pFile, "%d", &m_version);
    fclose(pFile);
    
    static const char *baseURL = "http://swmaestros-sng.appspot.com/systemversionchk?ver=%d";
    char url[256];
    sprintf(url, baseURL, m_version);
    
    CURL_DATA data;
    if(pNet->connectHttp(url, &data) != CURLE_OK)
    {
        m_isUpdated = false;
        return;
    }
         
    rapidxml::xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    if(strcmp(xmlDoc.first_node()->first_node()->value(), "true") == 0)
        m_isUpdated = true;
}

SystemInfo::~SystemInfo()
{
    if( m_isUpdated == false ) return;

    string filePath = CCFileUtils::sharedFileUtils()->getWriteablePath() + SYSTEM_FILE_NAME;
    
    FILE *pFile = fopen(filePath.data(), "wb");
    fprintf(pFile, "%d", m_version);
    fclose(pFile);
}

int SystemInfo::GetViersion()
{
    return m_version;
}

bool SystemInfo::isUpdatedVersion()
{
    return m_isUpdated;
}