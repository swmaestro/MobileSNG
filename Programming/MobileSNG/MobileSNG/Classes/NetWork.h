//
//  Network.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <stdlib.h>
#include <memory.h>
#include "curl.h"
#include <string>

#define NETWORK_RESPONSE_TIME 5

enum NETWORK_OBJECT {
    NETWORK_OBJECT_CONSTRUCTION,
    NETWORK_OBJECT_WAITTING,
    NETWORK_OBJECT_WORKING,
    NETWORK_OBJECT_DONE,
    NETWORK_OBJECT_FAIL,
    NETWORK_OBJECT_OTHER_WATTING
};

struct CURL_DATA {
    char    *pContent;
    int      size;
    
    CURL_DATA()
    {
        pContent    = (char*)malloc(1); //어짜피 realloc함 임시적
        size        = 0;
    }
    
    ~CURL_DATA()
    {
        free(pContent);
        pContent = NULL;
    }
};

class Network
{
private:
    CURL        *m_pCTX;
    bool         m_isMsgBoxOn;
    
public:
    Network(bool isMsgBoxOn = true);
    ~Network();

private:
    static int _WriteCurlDataCallback(void *ptr, int size, int nmemb, void *pData);
    
public:
    CURLcode    connectHttp(const char *url, CURL_DATA *pData);

public:
    int             GetResponseCode();
    std::string     GetContentType();
    double          GetSize();
    double          GetSpeed();
};