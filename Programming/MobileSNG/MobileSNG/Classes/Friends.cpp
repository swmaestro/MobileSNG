//
//  Friends.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 23..
//
//

#include "Friends.h"

using namespace cocos2d;

Friends::Friends(GameSystem * system)
{
    m_pSystem = system;
//    m_pSocial = new Social();
}

Friends::~Friends()
{
//    SAFE_DELETE(m_pSocial);
}

bool Friends::init()
{
    if (!CCLayer::init())
        return false;
    
    CCSprite * pBG = CCSprite::create("Friends-Background.png");
    pBG->setAnchorPoint(ccp(0, 0));
    pBG->setPosition(ccp(0, 0));
    pBG->setOpacity(150);
    addChild(pBG, 0);
    
    refresh();
    
    return true;
}

void Friends::refresh()
{
//    SAFE_DELETE(m_pSocial);
//    m_pSocial = new Social();
}