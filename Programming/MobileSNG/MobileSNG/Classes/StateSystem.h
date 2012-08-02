//
//  BuildingObject.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "DataType.h"
#include "ObjectInMapType.h"

class StateSystem : public OBJECT_IN_MAP
{
private:
    int                     m_innerTimer;
    
protected:
    int                     m_time;
    
public:
    StateSystem(objectState state, POINT<int> point, SIZE<int> size, OBJECT_DIRECTION direction, int _time);
    virtual ~StateSystem();

protected:
    void _update(int completeTime);
    
public:
    int GetTime();
    objectState GetState();
    bool isIn(int x, int y, int w, int h);
};