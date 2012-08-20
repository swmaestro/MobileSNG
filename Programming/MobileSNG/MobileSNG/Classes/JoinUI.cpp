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
    m_pBackGround->setPosition(ccp(0,0));
    m_pBackGround->setAnchorPoint(ccp(0,0));
    
    addChild(m_pBackGround);

    for (int i=0; i<JOIN_UI_ENUM_NUM; ++i)
    {
        m_pLabel[i] = new CCLabelTTF;
        m_pLabel[i]->setAnchorPoint(ccp(0,0));
        m_pLabel[i]->initWithString("", JOIN_UI_FONT_NAME, fontSize);
        addChild(m_pLabel[i]);
        
        m_pTextField[i] = new CCTextFieldTTF;
        m_pTextField[i]->initWithPlaceHolder("", JOIN_UI_FONT_NAME, fontSize);
        m_pTextField[i]->setHorizontalAlignment(kCCTextAlignmentLeft);
        m_pTextField[i]->setContentSize(CCSizeMake(200, 25));
        m_pTextField[i]->setAnchorPoint(ccp(0,0));
        m_pTextField[i]->setDelegate(this);
        addChild(m_pTextField[i]);
        
        printf("%f %f \n", m_pTextField[i]->getAnchorPoint().x, m_pTextField[i]->getAnchorPoint().y);
    }
    
    m_pLabel[JOIN_UI_ENUM_ID]->setPosition(ccp(50,160));
    m_pLabel[JOIN_UI_ENUM_ID]->setString("ID : ");
    m_pLabel[JOIN_UI_ENUM_PW]->setPosition(ccp(50,120));
    m_pLabel[JOIN_UI_ENUM_PW]->setString("PW : ");
    m_pLabel[JOIN_UI_ENUM_PHONE]->setPosition(ccp(50,80));
    m_pLabel[JOIN_UI_ENUM_PHONE]->setString("PHONE : ");

    m_pTextField[JOIN_UI_ENUM_ID]->setPosition(ccp(100, 150));
    m_pTextField[JOIN_UI_ENUM_PW]->setPosition(ccp(100,110));
    m_pTextField[JOIN_UI_ENUM_PHONE]->setPosition(ccp(100,70));
        
    CCMenuItemImage *pJoinButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, JoinSelector);
    pJoinButton->setPosition(ccp(0,0));
    pJoinButton->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage *pCancelButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, CancelSelector);
    pCancelButton->setPosition(ccp(60,0));
    pCancelButton->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage *pOverlabIDButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, OverlabSelector);
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
        boxPos.x = m_pTextField[i]->getPositionX();
        boxPos.y = m_pTextField[i]->getPositionY();
        boxSize.width = m_pTextField[i]->getContentSize().width;
        boxSize.height = m_pTextField[i]->getContentSize().height;
        
        if(intersectBoxWithPoint(boxPos, boxSize, touchPoint))
        {
            m_pTextField[i]->attachWithIME();
            break;
        }
        
        printf("%f %f \n", m_pTextField[i]->getAnchorPoint().x, m_pTextField[i]->getAnchorPoint().y);
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