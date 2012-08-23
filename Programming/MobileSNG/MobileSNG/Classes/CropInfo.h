//
//  CropType.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 17..
//
//

#pragma once

#include "CommonInfo.h"
#include "ObjectInfo.h"

class CropInfo : public CommonInfo
{
private:
    ObjectInfo      m_object;
    
public:
    CropInfo() : CommonInfo() { }
    CropInfo(ObjectInfo objectInfo, CommonInfo commonInfo) : CommonInfo(commonInfo)
    {
        m_object = objectInfo;
    }
    
public:
    inline ObjectInfo  GetObjInfo() { return m_object; }
};
    