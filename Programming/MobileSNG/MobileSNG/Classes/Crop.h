//
//  Crop.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Timer.h"
#include "ObjectInMap.h"

enum CROP_STATE
{
    CROP_STATE_GROW_1,
    CROP_STATE_GROW_2,
    CROP_STATE_GROW_3,
    CROP_STATE_DONE,
    CROP_STATE_INVAILD = -1
};

class Crop : public Timer
{
private:
    int                     m_id;
    objectState             m_state;
        
public:
    Crop(int id, int time);
    ~Crop();
    
public:
    void UpdateSystem(ObjectInfoMgr *pInfoMgr);
    
public:
    objectState GetState();
    int         GetID();
};