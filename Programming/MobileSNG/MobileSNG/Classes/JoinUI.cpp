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

JoinUI::JoinUI(const char* strBackGroundPath, CCNode *pTarget, cocos2d::SEL_MenuHandler JoinSelector, cocos2d::SEL_MenuHandler CancelSelector, SEL_MenuHandler OverlabSelector, int fontSize)
{
    m_pBackGround = new CCSprite;
    m_pBackGround->initWithFile(strBackGroundPath);
    m_pBackGround->setScale(0.5f);
    m_pBackGround->setPosition(ccp(0,0));
    m_pBackGround->setAnchorPoint(ccp(0,0));
    
    addChild(m_pBackGround);

    for (int i=0; i<JOIN_UI_ENUM_NUM; ++i)
    {
        m_pLabel[i] = new CCLabelTTF;
        m_pLabel[i]->setAnchorPoint(ccp(0,0));
        addChild(m_pLabel[i]);
        
        m_pTextField[i] = new CCTextFieldTTF;
        m_pTextField[i]->setAnchorPoint(ccp(0,0));
        m_pTextField[i]->setDelegate(this);
        addChild(m_pTextField[i]);
    }
    
    m_pLabel[JOIN_UI_ENUM_ID]->initWithString("ID : ", JOIN_UI_FONT_NAME, fontSize);
    m_pLabel[JOIN_UI_ENUM_ID]->setPosition(ccp(10,120));
        
    m_pLabel[JOIN_UI_ENUM_PW]->initWithString("PW : ", JOIN_UI_FONT_NAME, fontSize);
    m_pLabel[JOIN_UI_ENUM_PW]->setPosition(ccp(10,80));
    
    m_pLabel[JOIN_UI_ENUM_PHONE]->initWithString("Phone : ", JOIN_UI_FONT_NAME, fontSize);
    m_pLabel[JOIN_UI_ENUM_PHONE]->setPosition(ccp(10,40));
    
    m_pTextField[JOIN_UI_ENUM_ID]->initWithPlaceHolder("ID", CCSizeMake(200, 25), kCCTextAlignmentLeft, JOIN_UI_FONT_NAME, fontSize);
    m_pTextField[JOIN_UI_ENUM_ID]->cocos2d::CCNode::setPosition(80, 120);
      
    m_pTextField[JOIN_UI_ENUM_PW]->initWithPlaceHolder("PW", CCSizeMake(200, 25), kCCTextAlignmentLeft, JOIN_UI_FONT_NAME, fontSize);
    m_pTextField[JOIN_UI_ENUM_PW]->setPosition(ccp(80,80));
    
    m_pTextField[JOIN_UI_ENUM_PHONE]->initWithPlaceHolder("Phone", CCSizeMake(200, 25), kCCTextAlignmentLeft, JOIN_UI_FONT_NAME, fontSize);
    m_pTextField[JOIN_UI_ENUM_PHONE]->setPosition(ccp(80,40));
        
    CCMenuItemImage *pJoinButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, JoinSelector);
    pJoinButton->setPosition(ccp(0,0));
    pJoinButton->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage *pCancelButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, CancelSelector);
    pCancelButton->setPosition(ccp(100,0));
    pCancelButton->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage *pOverlabIDButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, JoinSelector);
    pOverlabIDButton->setPosition(ccp(120,0));
    pOverlabIDButton->setAnchorPoint(ccp(0,0));
    
    m_pMenu = CCMenu::create(pJoinButton, pCancelButton, pOverlabIDButton, NULL);
    m_pMenu->setAnchorPoint(ccp(0,0));
    m_pMenu->setPosition(ccp(0,0));

    addChild(m_pMenu);
    
    this->setTouchEnabled(true);
}

JoinUI::~JoinUI()
{
    removeAllChildrenWithCleanup(true);

    for(int i=0; i<JOIN_UI_ENUM_NUM; ++i)
    {
        delete m_pLabel[i];
        delete m_pTextField[i];
    }
    
    delete   m_pBackGround;
    
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

    //뭐 여기서 남은 작업해주면되
    POINT<int> boxPos;
    SIZE<int>  boxSize;
    POINT<int> touchPoint = POINT<int>(point.x, point.y);

    for (int i=0; i<JOIN_UI_ENUM_NUM; ++i) {
        boxPos = POINT<int>(m_pTextField[i]->getPositionX(), m_pTextField[i]->getPositionY());
        boxSize = SIZE<int>(m_pTextField[i]->getContentSize().width, m_pTextField[i]->getContentSize().height);
        
        if(intersectBoxWithPoint(boxPos, boxSize, touchPoint))
        {
            m_pTextField[i]->attachWithIME();
            break;
        }
    }
    
    printf("x %f, y %f \n", point.x, point.y);
}

void JoinUI::setEmptyTextField(JOIN_UI_ENUM e)
{
    m_pTextField[e]->setPlaceHolder("");
}

const char* JoinUI::GetContext(JOIN_UI_ENUM e)
{
    return m_pTextField[e]->getString();
}