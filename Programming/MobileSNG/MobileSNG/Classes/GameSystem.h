//
//  GameSystem.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 25..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "DataType.h"

class GameSystem
{
private:
    USER_INFO       *m_pUserInfo;
    
    
public:
    GameSystem(USER_INFO *pUserInfo);
    ~GameSystem();
};