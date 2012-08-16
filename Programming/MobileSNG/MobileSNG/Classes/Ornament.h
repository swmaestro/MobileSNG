//
//  Ornament.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ObjectInMap.h"

class Ornament : public ObjectInMap
{
  
public:
    Ornament(ObjectInMap *pObject);
    ~Ornament();
    
public:
    bool UpdateSystem();
};