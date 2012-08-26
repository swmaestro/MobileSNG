//
//  Talkbox.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 22..
//
//

#include "Talkbox.h"

using namespace cocos2d;

bool Talkbox::init()
{
    if (!CCLayer::init())
        return false;
    
    m_pScale = CCLayer::create();
    m_pScale->setAnchorPoint(ccp(0, 0));
    m_pScale->setPosition(0, 30);
    addChild(m_pScale);
    
    m_pSpr = CCSprite::create("Talkbox.png");
    m_pSpr->setAnchorPoint(ccp(0.5, 0));
    m_pScale->addChild(m_pSpr, 0);
    
    m_pText = CCLabelTTF::create("SuperJail!", "Ariel", 12);
    m_pText->setPosition(ccp(0, 50));
    m_pScale->addChild(m_pText, 1);
    
    return true;
}

void Talkbox::SetContent(std::string content)
{
    m_content = content;
    
    m_pText->setString(m_content.c_str());
    
    m_pScale->setScale(0);
    CCScaleTo * scale = CCScaleTo::create(0.1, 1);
    m_pScale->runAction(scale);
}