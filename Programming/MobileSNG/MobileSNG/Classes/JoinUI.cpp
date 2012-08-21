//
//  JoinUI.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 13..
//
//

#include "JoinUI.h"
#include "DataType.h"
#include "Utility.h"

using namespace cocos2d;

JoinUI::JoinUI(CCNode *pTarget, cocos2d::SEL_MenuHandler JoinSelector, cocos2d::SEL_MenuHandler CancelSelector, SEL_MenuHandler OverlabSelector, int fontSize)
{
    m_pBackGround = new CCSprite;
    m_pBackGround->initWithFile("TestUI.png");
    m_pBackGround->setPosition(ccp(0,0));
    m_pBackGround->setAnchorPoint(ccp(0,0));
    
    addChild(m_pBackGround);
    
    setContentSize(m_pBackGround->getContentSize());

    for (int i=0; i<JOIN_UI_ENUM_NUM; ++i)
    {
        m_pLabel[i] = new CCLabelTTF;
        m_pLabel[i]->setAnchorPoint(ccp(0,0));
        m_pLabel[i]->initWithString("", JOIN_UI_FONT_NAME, fontSize);
        m_pLabel[i]->setColor(ccc3(0,0,0));
        addChild(m_pLabel[i]);
        
        m_pTextField[i] = new CCTextFieldTTF;
        m_pTextField[i]->initWithPlaceHolder("", JOIN_UI_FONT_NAME, fontSize);
        m_pTextField[i]->setHorizontalAlignment(kCCTextAlignmentLeft);
        m_pTextField[i]->setContentSize(CCSizeMake(200, 30));
        m_pTextField[i]->setAnchorPoint(ccp(0,0));
        m_pTextField[i]->setDelegate(this);
        m_pTextField[i]->setColor(ccc3(0,0,0));
        addChild(m_pTextField[i]);
        
        printf("%f %f \n", m_pTextField[i]->getAnchorPoint().x, m_pTextField[i]->getAnchorPoint().y);
    }
    
    m_pLabel[JOIN_UI_ENUM_ID]->setPosition(ccp(50,130));
    m_pLabel[JOIN_UI_ENUM_ID]->setString("ID : ");
    m_pLabel[JOIN_UI_ENUM_PW]->setPosition(ccp(50,100));
    m_pLabel[JOIN_UI_ENUM_PW]->setString("PW : ");
    m_pLabel[JOIN_UI_ENUM_PHONE]->setPosition(ccp(50,70));
    m_pLabel[JOIN_UI_ENUM_PHONE]->setString("PHONE : ");

    m_pTextField[JOIN_UI_ENUM_ID]->setPosition(ccp(100, 115));
    m_pTextField[JOIN_UI_ENUM_PW]->setPosition(ccp(100,85));
    m_pTextField[JOIN_UI_ENUM_PHONE]->setPosition(ccp(100,55));
        
    CCMenuItemImage *pJoinButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, JoinSelector);
    pJoinButton->setPosition(ccp(110,0));
    pJoinButton->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage *pCancelButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, CancelSelector);
    pCancelButton->setPosition(ccp(170,0));
    pCancelButton->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage *pOverlabIDButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, OverlabSelector);
    pOverlabIDButton->setPosition(ccp(230,0));
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

bool JoinUI::onTextFieldInsertText(cocos2d::CCTextFieldTTF * sender, const char * text, int nLen)
{
    //여기서 번호 텍스트 필드라면 - 처리 해주라
    return false;
}

void JoinUI::ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent)
{
    TextFieldKeyBoardOn(this, pTouches, m_pTextField, JOIN_UI_ENUM_NUM);
}

void JoinUI::setEmptyTextField(JOIN_UI_ENUM e)
{
    m_pTextField[e]->setPlaceHolder("");
}

const char* JoinUI::GetString(JOIN_UI_ENUM e)
{
    return m_pTextField[e]->getString();
}