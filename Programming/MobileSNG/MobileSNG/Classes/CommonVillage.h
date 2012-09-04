//
//  CommonVillage.h
//  MobileSNG
//
//  Created by 박 진 on 12. 9. 2..
//
//

#pragma once

#include "Network.h"
#include "rapidxml.hpp"
#include <vector>
#include "ObjectInMap.h"
#include "DateInfo.h"

class CommonVillage
{
protected:
    Network         *m_pNetwork;
    ObjectInfoMgr   *m_pInfoMgr;
    
public:
    CommonVillage(Network *pNetwork);
    virtual ~CommonVillage();
    
protected:
    void _getCropState(objectState *pState);
    void _getObjectState(objectState *pState);
    
    bool _networkNormalResult(CURL_DATA *pData);

    std::vector< std::pair<ObjectInMap, long long int> > _parseBuildingInVillage(const char* pContent);
    std::vector< std::pair<int, int> > _parseCropInVillage(const char* pContent);
    
    
    bool        _getServerTime(DateInfo *pInfo);
};