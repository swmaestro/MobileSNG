
//
//  Ornament.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 26..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Ornament.h"

Ornament::Ornament(ObjectInMap *pObject) 
               :ObjectInMap(pObject)
{
    m_type = OBJECT_TYPE_ORNAMENT;
}

Ornament::~Ornament()
{
    
}

bool Ornament::UpdateSystem()
{
    return false;
}