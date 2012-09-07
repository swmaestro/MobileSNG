//
//  Friends.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 23..
//
//

#include "Friends.h"

using namespace cocos2d;

Friends::Friends(GameSystem * system, Network *pNetwork)
{
    m_pNetwork = pNetwork;
    m_pSystem = system;
    m_pSocial = NULL;
    m_pTextField = NULL;
}

Friends::~Friends()
{
    SAFE_DELETE(m_pSocial);
    SAFE_DELETE(m_pTextField);
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
    
    m_pSocial = new Social(m_pNetwork, m_pSystem->GetPlayer());

    m_pMovable = CCLayer::create();
    addChild(m_pMovable, 1);
    
    m_pTextField = new CCTextFieldTTF;
    m_pTextField->initWithPlaceHolder("", "Ariel", 20);
    m_pTextField->setDelegate(this);
    m_pTextField->setAnchorPoint(ccp(0, 0));
    m_pTextField->setPosition(ccp(0, 300));
    m_pTextField->setColor(ccc3(255, 255, 255));
    m_pTextField->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_pTextField->setContentSize(CCSizeMake(400, 20));
    addChild(m_pTextField, 1);
    
    refresh();
    
    return true;
}

void Friends::addFriendList(std::string name, int num)
{
    CCLayer * layer = CCLayer::create();
    
    CCSprite * bg = CCSprite::create("Friends-List.png");
    bg->setAnchorPoint(ccp(0, 0));
    layer->addChild(bg);
    
    CCLabelTTF * lb = CCLabelTTF::create(name.c_str(), "Ariel", 20);
    lb->setPosition(ccp(20, 30));
    layer->addChild(lb);
    
    layer->setPosition(ccp(0, 320 - 90 - num * 45));
    m_pMovable->addChild(layer);
}

void Friends::refresh()
{
    m_pMovable->removeAllChildrenWithCleanup(true);
    
    std::vector<VillageInfo *> v = m_pSocial->GetFollowing(m_pSystem->GetPlayer()->GetUserID(), 0);
    std::vector<VillageInfo *>::iterator i;
    int c = 0;
    
    for (i = v.begin(); i != v.end(); ++i, ++c)
        addFriendList((*i)->userID, c);
}

void Friends::refreshBySearch(std::string name)
{
    m_pMovable->removeAllChildrenWithCleanup(true);
    
    VillageInfo info;
    if (m_pSocial->FindVillageInfo(name.c_str(), &info))
        addFriendList(info.userID);
}

void Friends::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    TextFieldKeyBoardOn(this, pTouches, &m_pTextField);
}

bool Friends::onTextFieldInsertText(cocos2d::CCTextFieldTTF * sender, const char * text, int nLen)
{
    if (text[0] == '\n')
        refreshBySearch(sender->getString());
        
    return false;
}