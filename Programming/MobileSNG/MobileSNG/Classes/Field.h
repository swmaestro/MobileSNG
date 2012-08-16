//
//  Field.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 29..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Crop.h"
#include "ObjectInMap.h"

class Field : public ObjectInMap
{
private:
    Crop              *m_pCrop;
    
public:
    Field(ObjectInMap *pObject);
    ~Field();
    
public:
    bool UpdateSystem();
    
public:
    bool addCrop(int id, int time, ObjectInfoMgr *pInfoMgr);
    void removeCrop();

public:
    Crop*   GetCrop();
    
public:
    bool hasCrop();
};