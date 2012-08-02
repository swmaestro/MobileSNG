//
//  Shop.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 23..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cocos2d.h"

class Shop : public cocos2d::CCLayer
{
private:
    cocos2d::CCSprite       * m_pBackGround;
    
public:
    Shop();
    ~Shop();
    
    bool init();
};