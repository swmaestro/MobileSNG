//
//  JoinUI.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 13..
//
//

#include "JoinUI.h"
#include "DataType.h"

using namespace cocos2d;

JoinUI::JoinUI(const char* strBackGroundPath, CCNode *pTarget, cocos2d::SEL_MenuHandler JoinSelector, cocos2d::SEL_MenuHandler CancelSelector,  int fontSize)
{
    m_pBackGround = new CCSprite;
    m_pBackGround->initWithFile(strBackGroundPath);
    m_pBackGround->setScale(0.5f);
    m_pBackGround->setPosition(ccp(0,0));
    m_pBackGround->setAnchorPoint(ccp(0,0));
    
    addChild(m_pBackGround);
    
    m_pIDLabel = new CCLabelTTF;
    m_pIDLabel->initWithString("ID : ", JOIN_UI_FONT_NAME, fontSize);
    m_pIDLabel->setPosition(ccp(10,120));
    m_pIDLabel->setAnchorPoint(ccp(0,0));
    
    addChild(m_pIDLabel);
    
    m_pPassWordLabel = new CCLabelTTF;
    m_pPassWordLabel->initWithString("PW : ", JOIN_UI_FONT_NAME, fontSize);
    m_pPassWordLabel->setPosition(ccp(10,80));
    m_pPassWordLabel->setAnchorPoint(ccp(0,0));
    
    addChild(m_pPassWordLabel);
    
    m_pIDTextField = new CCTextFieldTTF;
    m_pIDTextField->initWithPlaceHolder("Test", CCSizeMake(200, 25), kCCTextAlignmentLeft, JOIN_UI_FONT_NAME, fontSize);
    m_pIDTextField->setAnchorPoint(ccp(0,0));
    m_pIDTextField->cocos2d::CCNode::setPosition(80, 120);
    m_pIDTextField->setDelegate(this);
    m_pIDTextField->attachWithIME();
//    m_pIDTextField->detachWithIME();
    
    addChild(m_pIDTextField);
  
    m_pPWTextField = new CCTextFieldTTF;
    m_pPWTextField->initWithPlaceHolder("Test", CCSizeMake(200, 25), kCCTextAlignmentLeft, JOIN_UI_FONT_NAME, fontSize);
    m_pPWTextField->setAnchorPoint(ccp(0,0));
    m_pPWTextField->setPosition(ccp(80,80));
    m_pPWTextField->setDelegate(this);
    
    addChild(m_pPWTextField);
    
    CCMenuItemImage *pJoinButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, JoinSelector);
    pJoinButton->setPosition(ccp(0,0));
    pJoinButton->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage *pCancelButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, CancelSelector);
    pCancelButton->setPosition(ccp(100,0));
    pCancelButton->setAnchorPoint(ccp(0,0));
    
    m_pMenu = CCMenu::create(pJoinButton, pCancelButton, NULL);
    m_pMenu->setAnchorPoint(ccp(0,0));
    m_pMenu->setPosition(ccp(0,0));

    addChild(m_pMenu);
    
    this->setTouchEnabled(true);
    
}

JoinUI::~JoinUI()
{
    removeAllChildrenWithCleanup(true);
    delete m_pPassWordLabel;
    delete m_pIDLabel;
    delete m_pBackGround;
    delete m_pIDTextField;
    delete m_pPWTextField;
    
    m_pMenu->removeAllChildrenWithCleanup(true);
}

bool JoinUI::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
    return false;
}

void JoinUI::ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent)
{
    CCPoint point;
    CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
    
    point = pTouch->locationInView();
    point = CCDirector::sharedDirector()->convertToGL(point);
    point.x -= getPositionX();
    point.y -= getPositionY();
    
    CCSize size = m_pBackGround->getTexture()->getContentSize();
    size.width *= getScaleX();
    size.height *= getScaleY();
    
    //if(intersectBoxWithPoint(POINT<int>(0,0), SIZE<int>(size.width, size.height), POINT<int>(point.x, point.y)) == false) return;
    
    //뭐 여기서 남은 작업해주면되
    POINT<int> boxPos  = POINT<int>(m_pIDTextField->getPositionX(), m_pIDTextField->getPositionY());
    SIZE<int>  boxSize = SIZE<int>(m_pIDTextField->getContentSize().width, m_pIDTextField->getContentSize().height);
    POINT<int> touchPoint = POINT<int>(point.x, point.y);
    
    if(intersectBoxWithPoint(boxPos, boxSize, touchPoint))
        m_pIDTextField->attachWithIME();
    
    boxPos      = POINT<int>(m_pPWTextField->getPositionX(), m_pPWTextField->getPositionY());
    boxSize     = SIZE<int>(m_pPWTextField->getContentSize().width, m_pPWTextField->getContentSize().height);

    if(intersectBoxWithPoint(boxPos, boxSize, touchPoint))
        m_pPWTextField->attachWithIME();
    
    printf("x %f, y %f \n", point.x, point.y);
}

const char* JoinUI::GetID()
{
    return m_pIDTextField->getString();
}

const char* JoinUI::GetPassWord()
{
    return m_pPWTextField->getString();
}