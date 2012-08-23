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
    
    CCSprite * spr = CCSprite::create("Talkbox.png");
    spr->setAnchorPoint(ccp(0.5, 0));
    addChild(spr, 0);
    
    m_pText = CCLabelTTF::create("SuperJail!", "Ariel", 15);
    m_pText->setPosition(ccp(0, 50));
    addChild(m_pText, 1);
    
    return true;
}

void Talkbox::SetContent(std::string content)
{
    m_content = content;
    
    m_pText->setString(m_content.c_str());
}