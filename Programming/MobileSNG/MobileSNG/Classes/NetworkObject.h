//
//  NetworkBuildingInfo.h
//  MobileSNG
//
//  Created by 박 진 on 12. 9. 4..
//
//

#pragma once

#include "DateInfo.h"
#include "ObjectInMap.h"
#include "Utility.h"

struct NetworkObject
{
    int                 index;
    int                 id;
    objectState         state;
    POINT<int>          position;
    OBJECT_DIRECTION    direction;
    DateInfo            date;
    
    NetworkObject() : index(0), id(0), state(0), position(0,0), direction(OBJECT_DIRECTION_LEFT){}
    
    NetworkObject(int index, int id, objectState state, POINT<int> position, OBJECT_DIRECTION dir, DateInfo date)
    {
        this->index     = index;
        this->id        = id;
        this->state     = state;
        this->position  = position;
        this->direction = direction;
        this->date      = date;
    }
};