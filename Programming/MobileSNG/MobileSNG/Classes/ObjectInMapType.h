//
//  ObjectInMapType.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "DataType.h"

typedef int objectState;

typedef enum OBJECT_DIRECTION
{
    OBJECT_DIRECTION_LEFT,
    OBJECT_DIRECTION_RIGHT
}OBJECT_DIRECTION;

typedef enum OBJECT_TYPE
{
    OBJECT_TYPE_NONE,
    OBJECT_TYPE_BUILDING,
    OBJECT_TYPE_CROP,
    OBJECT_TYPE_ORNAMENT
}OBJECT_TYPE;