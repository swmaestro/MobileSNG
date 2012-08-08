//
//  NetWork.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "NetWork.h"

NetWork::NetWork(const char *url, CURL_DATA *pData)
{
    curl_global_init(CURL_GLOBAL_ALL);
    
    m_pCTX = curl_easy_init();
    
    curl_easy_setopt(m_pCTX, CURLOPT_URL, url);
    curl_easy_setopt(m_pCTX, CURLOPT_WRITEDATA, pData);
    curl_easy_setopt(m_pCTX, CURLOPT_WRITEFUNCTION, 
                     NetWork::WriteCurlDataCallback);
}

NetWork::~NetWork()
{
    curl_easy_cleanup(m_pCTX);
    curl_global_cleanup();
    
    m_pCTX = NULL;
}

CURLcode NetWork::connectHttp()
{
    return curl_easy_perform(m_pCTX);
}

int NetWork::WriteCurlDataCallback(void *ptr, int size, 
                                         int nmemb, void *pData)
{
    int realSize = size *nmemb;
    CURL_DATA *pCurlData = static_cast<CURL_DATA*>(ptr);

    pCurlData->pContent = (char*)realloc(&(pCurlData->pContent[pCurlData->size]), pCurlData->size + realSize + 1);
    
    if (pCurlData->pContent == NULL) {
        return 0;
    }
    
    memcpy(&(pCurlData->pContent[pCurlData->size]), ptr, realSize);
    pCurlData->size += realSize;
    pCurlData->pContent[pCurlData->size] = 0;
    
    return realSize;
}