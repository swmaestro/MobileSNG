//
//  NetWork.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <stdlib.h>
#include <memory.h>
#include "curl.h"

struct CURL_DATA {
    char    *pContent;
    int      size;
    
    CURL_DATA()
    {
        pContent    = (char*)malloc(1); //어짜피 realloc함 임시적
        pContent[0] = NULL;
        size        = 0;
    }
    
    ~CURL_DATA()
    {
        free(pContent);
        pContent = NULL;
    }
};

class NetWork
{
private:
    CURL        *m_pCTX;
    
public:
    NetWork(const char *url, CURL_DATA *pData);
    ~NetWork();
    
public:
    CURLcode    connectHttp();
    
public:
    static int WriteCurlDataCallback(void *ptr, int size, int nmemb, void *pData);
};