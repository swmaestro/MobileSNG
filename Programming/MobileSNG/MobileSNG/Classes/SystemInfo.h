//
//  SystemInfo.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 14..
//
//

#pragma once

#include "DataType.h"
#include "Network.h"

#define SYSTEM_FILE_NAME "System.info"

class SystemInfo
{
private:
    bool                m_isUpdated;
    
private:
    int                 m_version;
    
public:
    SystemInfo(Network *pNet);
    ~SystemInfo();
    
public:
    int             GetViersion();
    bool            isUpdatedVersion();
};