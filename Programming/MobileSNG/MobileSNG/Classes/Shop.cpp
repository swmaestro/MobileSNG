//
//  Shop.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 23..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Shop.h"

using namespace cocos2d;

Shop::Shop()
{
    m_pBackGround = NULL;
}

Shop::~Shop()
{
    removeAllChildrenWithCleanup(true);
}

bool Shop::init()
{  
    if (!CCLayer::init())
        return false;
    
    m_pBackGround = CCSprite::create("Shop-BackGround.png");
    m_pBackGround->setAnchorPoint(ccp(0, 0));
    m_pBackGround->setPosition(ccp(0, 0));

    addChild(m_pBackGround);
    
    return true;
}