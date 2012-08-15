//
//  SystemInfo.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 14..
//
//

#include "SystemInfo.h"

#include "CCFileUtils.h"
#include <stdlib.h>

using namespace cocos2d;
using namespace std;

SystemInfo::SystemInfo(const char *FileName)
{
    m_strFilePath = CCFileUtils::sharedFileUtils()->getWriteablePath().append(FileName);
    
    FILE *pFile = fopen(m_strFilePath.data(), "rb");
    
    if(pFile)
    {
        char txt[256];
        int  num;
        
        fscanf(pFile, "%d", &m_version);
        
        fscanf(pFile, "%d", &num);

        m_date.year     = num / 10000;
        m_date.month    = (num - (m_date.year * 10000))/100;
        m_date.day      = num % 100;
        
        fscanf(pFile, "%s", txt);
        m_updateLog = txt;
    }
    
    fclose(pFile);
    
    m_isReWrite = false;
}

SystemInfo::~SystemInfo()
{
    if( m_isReWrite == false ) return;
    
    FILE *pFile = fopen(m_strFilePath.data(), "wb");
    
    fprintf(pFile, "%d\n", m_version);
    fprintf(pFile, "%d%02d%02d\n", m_date.year, m_date.month, m_date.day);
    fprintf(pFile, "%s\n",m_updateLog.data());

    fclose(pFile);
}

void SystemInfo::UpdateInfo(int version, char *xmlData)
{
    if( (m_isReWrite = (m_version != version)) == false ) return;
    
    //추후 작업.
    //일단 맨 처음에 시스템 정보에 관한걸 받을테니까 이렇게 해둠.
    //물론 언제든 바낄 수 있음.
}

int SystemInfo::GetViersion()
{
    return m_version;
}

DATE SystemInfo::GetDate()
{
    return m_date;
}

const char* SystemInfo::GetLog()
{
    return m_updateLog.data();
}