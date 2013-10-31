//
//  Network.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Network.h"
#include "CCCommon.h"

Network::Network(bool isMsgBoxOn)
{
    m_isMsgBoxOn = isMsgBoxOn;
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    m_pCTX = curl_easy_init();    
}

Network::~Network()
{
    curl_easy_cleanup(m_pCTX);
    curl_global_cleanup();
    
    m_pCTX = NULL;
}

CURLcode Network::connectHttp(const char *url, CURL_DATA *pData)
{
    curl_easy_setopt(m_pCTX, CURLOPT_URL, url);
    curl_easy_setopt(m_pCTX, CURLOPT_WRITEDATA, pData);
    curl_easy_setopt(m_pCTX, CURLOPT_WRITEFUNCTION,
                     Network::_WriteCurlDataCallback);
    
    curl_easy_setopt(m_pCTX, CURLOPT_CONNECTTIMEOUT, NETWORK_RESPONSE_TIME/2);
    curl_easy_setopt(m_pCTX, CURLOPT_TIMEOUT, NETWORK_RESPONSE_TIME);
    
    CURLcode code = curl_easy_perform(m_pCTX);

    if( code != CURLE_OK && m_isMsgBoxOn )
        cocos2d::CCMessageBox("서버에 연결되지 않습니다.", "Error");

    return code;
}

int Network::_WriteCurlDataCallback(void *ptr, int size, 
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

int Network::GetResponseCode()
{
    int code;
    
    if( curl_easy_getinfo(m_pCTX, CURLINFO_HTTP_CODE, &code) == CURLE_OK)
        return code;
    
    return -1;
}

std::string Network::GetContentType()
{
    char type[256];
    
    if(curl_easy_getinfo(m_pCTX, CURLINFO_CONTENT_TYPE, type) == CURLE_OK)
        return type;
    
    return NULL;
}

double Network::GetSize()
{
    double code;
    
    if(curl_easy_getinfo(m_pCTX, CURLINFO_SIZE_DOWNLOAD, &code) == CURLE_OK)
        return code;
    
    return -1.f;
}

double Network::GetSpeed()
{
    double code;
    
    if( curl_easy_getinfo(m_pCTX, CURLINFO_SPEED_DOWNLOAD, &code) == CURLE_OK)
        return code;
    
    return -1.f;
}
