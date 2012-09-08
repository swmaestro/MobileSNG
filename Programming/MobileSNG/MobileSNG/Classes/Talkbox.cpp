//
//  Talkbox.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 22..
//
//

#include "Talkbox.h"
#include "Utility.h"

using namespace cocos2d;

Talkbox::Talkbox() : m_pScale(NULL), m_pTalkbox(NULL), m_pRemove(NULL)
{
    memset(m_pText, 0, sizeof(CCLabelTTF*) * 3);
}

Talkbox::~Talkbox()
{
    m_pScale->removeAllChildrenWithCleanup(true);
    removeAllChildrenWithCleanup(true);
    
    SAFE_DELETE(m_pScale);
    SAFE_DELETE(m_pTalkbox);
    SAFE_DELETE(m_pRemove);
    
    for(int i=0; i<3; i++)
        SAFE_DELETE(m_pText[i]);
}

bool Talkbox::init()
{
    if (!CCLayer::init())
        return false;
    
    m_pScale = new CCLayer;
    m_pScale->init();
    m_pScale->setAnchorPoint(ccp(0, 0));
    m_pScale->setPosition(0, 30);
    addChild(m_pScale);
    
    m_pTalkbox = new CCSprite;//CCSprite::create("Talkbox.png");
    m_pTalkbox->initWithFile("Talkbox.png");
    m_pTalkbox->setAnchorPoint(ccp(0.5, 0));
    m_pScale->addChild(m_pTalkbox, 0);
    
    m_pRemove = new CCSprite;//CCSprite::create("Remove.png");
    m_pRemove->initWithFile("Remove.png");
    m_pRemove->setPosition(ccp(40, 60));
    m_pScale->addChild(m_pRemove, 1);
    
    for(int i=0; i<3; ++i)
    {
        m_pText[i] = new CCLabelTTF;
        m_pText[i]->initWithString("SuperJail!", "MarkerFelt", 12);
        m_pText[i]->setPosition(ccp(0, 50));
        m_pText[i]->setVisible(false);
        m_pScale->addChild(m_pText[i], 1);
    }
    
    m_pText[TALKBOX_TYPE_FARM]->setString("Faaaaaaaaarm");
    m_pText[TALKBOX_TYPE_BUILDING]->setString("Buuuuuilding");
    m_pText[TALKBOX_TYPE_UNKNOWN]->setString("Error : I don't know\nwhat is this");
    
    this->setVisible(false);
    m_pos = ccp(0, 0);
    
    return true;
}

void Talkbox::SetContent(CCPoint pos, TALKBOX_TYPE type)
{
    m_pos = pos;
    
    for(int i=0; i<3; i++)
        m_pText[i]->setVisible(false);
    
    m_pText[type]->setVisible(true);
    this->setVisible(true);
    
    m_pScale->setScale(0);
    CCScaleTo * scale = CCScaleTo::create(0.1, 1);
    m_pScale->runAction(scale);
}

bool Talkbox::Touch(CCTouch *touch)
{
    if (!isVisible())
        return false;
    
    CCPoint p = m_pTalkbox->convertTouchToNodeSpace(touch);
    
    if (0 > p.x || p.x > 120 || 20 > p.y || p.y > 70)
        return false;

    return true;
}