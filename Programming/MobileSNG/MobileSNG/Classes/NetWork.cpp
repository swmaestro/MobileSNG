//
//  NetWork.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "NetWork.h"

NetWork::NetWork()
{
    curl_global_init(CURL_GLOBAL_ALL);
    
    m_pCTX = curl_easy_init();    
}

NetWork::~NetWork()
{
    curl_easy_cleanup(m_pCTX);
    curl_global_cleanup();
    
    m_pCTX = NULL;
}

CURLcode NetWork::connectHttp(const char *url, CURL_DATA *pData)
{
    curl_easy_setopt(m_pCTX, CURLOPT_URL, url);
    curl_easy_setopt(m_pCTX, CURLOPT_WRITEDATA, pData);
    curl_easy_setopt(m_pCTX, CURLOPT_WRITEFUNCTION,
                     NetWork::_WriteCurlDataCallback);
    
    curl_easy_setopt(m_pCTX, CURLOPT_CONNECTTIMEOUT, NETWORK_RESPONSE_TIME/2);
    curl_easy_setopt(m_pCTX, CURLOPT_TIMEOUT, NETWORK_RESPONSE_TIME);
    
    return curl_easy_perform(m_pCTX);
}

int NetWork::_WriteCurlDataCallback(void *ptr, int size, 
                                         int nmemb, void *pData)
{
    int realSize = size * nmemb;
    CURL_DATA *pCurlData = static_cast<CURL_DATA*>(pData);

    pCurlData->pContent = (char*)realloc(pCurlData->pContent, pCurlData->size + realSize + 1);
    
    if (pCurlData->pContent == NULL) {
        return 0;
    }
    
    memcpy(&(pCurlData->pContent[pCurlData->size]), ptr, realSize);
    pCurlData->size += realSize;
    pCurlData->pContent[pCurlData->size] = 0;
    
    return realSize;
}

int NetWork::GetResponseCode()
{
    int code;
    
    if( curl_easy_getinfo(m_pCTX, CURLINFO_HTTP_CODE, &code) == CURLE_OK)
        return code;
    
    return -1;
}

bool NetWork::GetContentType(char *pOutStr)
{
    if(curl_easy_getinfo(m_pCTX, CURLINFO_CONTENT_TYPE, pOutStr) == CURLE_OK)
        return true;
    
    return false;
}

double NetWork::GetSize()
{
    double code;
    
    if(curl_easy_getinfo(m_pCTX, CURLINFO_SIZE_DOWNLOAD, &code) == CURLE_OK)
        return code;
    
    return -1.f;
}

double NetWork::GetSpeed()
{
    double code;
    
    if( curl_easy_getinfo(m_pCTX, CURLINFO_SPEED_DOWNLOAD, &code) == CURLE_OK)
        return code;
    
    return -1.f;
}
