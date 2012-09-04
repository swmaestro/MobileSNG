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
    Timer             *m_pTimer;
    
public:
    Field(ObjectInMap *pObject, int index);
    ~Field();
    
public:
    bool UpdateSystem();
    
public:
    Crop* addCrop(int id, int time, ObjectInfoMgr *pInfoMgr);
    void removeCrop();

public:
    Crop*   GetCrop();
    
public:
    bool hasCrop();
    bool isDone();
    
public:
    Timer* GetTimer() { return m_pTimer; }
};