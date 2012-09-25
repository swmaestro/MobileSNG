//
//  GameMessageBox.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 9. 12..
//
//

#include "GameMessageBox.h"
#include "Utility.h"

using namespace cocos2d;

GameMessageBox::GameMessageBox()
{
    m_pBackGround = NULL;
    m_pOK = NULL;
    
    memset(m_pGameMsg, 0, sizeof(CCLabelTTF*) * 3);
}

GameMessageBox::~GameMessageBox()
{
    removeAllChildrenWithCleanup(m_pBackGround);
    removeAllChildrenWithCleanup(true);
    
    SAFE_DELETE(m_pBackGround);
    SAFE_DELETE(m_pOK);
    
    for(int i=0; i<3; ++i)
        SAFE_DELETE(m_pGameMsg[i]);
}

bool GameMessageBox::init(CCTexture2D *pTex, CCTexture2D *pOK)
{
    if (!CCLayer::init())
        return false;

    m_pBackGround = new CCSprite;
    m_pBackGround->initWithTexture(pTex);
    m_pBackGround->setAnchorPoint(ccp(0.5f,0.5f));
    m_pBackGround->setPosition(ccp(240, 160));

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(size);
    
    addChild(m_pBackGround);

    m_pOK = new CCSprite;
    m_pOK->initWithTexture(pOK);
    m_pOK->setAnchorPoint(ccp(0.5f, 0.5f));
    
    CCPoint p;
    
    p.x = this->getPosition().x + this->getContentSize().width  / 2;
    p.y = this->getPosition().y + this->getContentSize().height / 2 - 45;
    
    m_pOK->setPosition(p);
    m_pBackGround->addChild(m_pOK);
    
    const char *gameMsg[3];
    
    gameMsg[GAMEMSGBOX_ENUM_ADD_OBJECT_ERROR]    = "건물/밭 생성에 실패하였습니다.";
    gameMsg[GAMEMSGBOX_ENUM_ADD_CROP_ERROR]      = "작물 심기에 실패하였습니다";
    gameMsg[GAMEMSGBOX_ENUM_SELL_ERROR]          = "판매에 실패하였습니다";
    
    for(int i=0; i<3; ++i)
    {
        m_pGameMsg[i] = new CCLabelTTF;
        m_pGameMsg[i]->initWithString(gameMsg[i], "MarkerFelt", 14);
        m_pGameMsg[i]->setPosition(ccp(0, 50));
        m_pGameMsg[i]->setVisible(false);
        m_pBackGround->addChild(m_pGameMsg[i], GAMEMSGBOX_ENUM_NONE + i);
    }
        
    return true;
}

bool GameMessageBox::btOKTouch(CCTouch *pTouch)
{
    if (!isVisible())
        return false;
    
    CCPoint p = m_pOK->convertTouchToNodeSpace(pTouch);
    
    if (0 > p.x || p.x > 120 || 20 > p.y || p.y > 70)
        return false;
    
    return true;
}