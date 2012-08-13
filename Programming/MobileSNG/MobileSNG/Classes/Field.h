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
    void             (*m_pStateChangeCallBack)(ObjectInMap*, objectState);
    
public:
    Field(ObjectInMap *pObject, void (*stateChangeCAllBack)(ObjectInMap* pObj, objectState state) = NULL);
    ~Field();
    
public:
    void UpdateSystem(ObjectInfoMgr *pInfoMgr);
    
public:
    bool addCrop(int id, int time = 0);
    void removeCrop();

public:
    Crop*   GetCrop();
    
public:
    bool hasCrop();
};