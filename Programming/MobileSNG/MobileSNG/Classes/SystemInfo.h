//
//  SystemInfo.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 14..
//
//

#pragma once

#include "DataType.h"

#define SYSTEM_FILE_NAME "System.info"

class SystemInfo
{
private:
    std::string         m_strFilePath;
    bool                m_isReWrite;
    
private:
    int                 m_version;
    DATE                m_date;
    std::string         m_updateLog;
    
public:
    SystemInfo(const char *FileName = SYSTEM_FILE_NAME);
    ~SystemInfo();
    
public:
    void UpdateInfo(int version, char *xmlData = NULL);
    
public:
    int             GetViersion();
    DATE            GetDate();
    const char*     GetLog();
};