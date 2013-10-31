//
//  BuildingType.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 17..
//
//

#pragma once

#include "Utility.h"
#include "CommonInfo.h"
#include "ObjectInfo.h"

class BuildingInfo : public CommonInfo
{
private:
    SIZE<int>       m_size;
    ObjectInfo      m_object;
    int             m_buildTime;

public:
    BuildingInfo() : CommonInfo() { }
    BuildingInfo(ObjectInfo objectInfo, CommonInfo commonInfo, SIZE<int> buildingSize, int buildTime) :  CommonInfo(commonInfo)
    {
        m_size            = buildingSize;
        m_object          = objectInfo;
        this->m_buildTime = buildTime;
    }
    
public:
    inline SIZE<int>       GetSize()       { return m_size;        }
    inline ObjectInfo      GetObjInfo()    { return m_object;      }
    inline int             GetBuildTime()  { return m_buildTime;   }
};