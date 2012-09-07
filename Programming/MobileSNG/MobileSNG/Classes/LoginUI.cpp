//
//  LoginUI.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 21..
//
//

#include "LoginUI.h"
#include "Utility.h"

using namespace cocos2d;

LoginUI::LoginUI(SEL_MenuHandler loginSelector, SEL_MenuHandler joinSelector, CCObject *pTarget, int fontSize)
{
    m_pBackGround = new CCSprite;
    m_pBackGround->initWithFile("Login-Background.png");
    m_pBackGround->setPosition(ccp(-10, -10));
    m_pBackGround->setAnchorPoint(ccp(0, 0));
    
    addChild(m_pBackGround);
    
    for(int i=0; i<LOGIN_UI_ENUM_NUM; ++i)
    {
        m_pTextField[i] = new CCTextFieldTTF;
        m_pTextField[i]->initWithPlaceHolder("", LOGIN_UI_FONT_NAME, fontSize);
        m_pTextField[i]->setAnchorPoint(ccp(0,0));
        m_pTextField[i]->setColor(ccc3(0, 0, 0));
        m_pTextField[i]->setHorizontalAlignment(kCCTextAlignmentLeft);
        m_pTextField[i]->setContentSize(LOGIN_UI_TEXTFILED_SIZE);
        m_pTextField[i]->setDelegate(this);
        
        m_pLabel[i] = new CCLabelTTF;
        m_pLabel[i]->initWithString("", LOGIN_UI_FONT_NAME, fontSize);
        m_pLabel[i]->setColor(ccc3(0, 0, 0));
        m_pLabel[i]->setAnchorPoint(ccp(0,0));
        
        addChild(m_pTextField[i]);
        addChild(m_pLabel[i]);
    }
    
    m_pLabel[LOGIN_UI_ENUM_ID]->setString("ID : ");
    m_pLabel[LOGIN_UI_ENUM_ID]->setPosition(ccp(10,110));
    
    m_pLabel[LOGIN_UI_ENUM_PW]->setString("PW : ");
    m_pLabel[LOGIN_UI_ENUM_PW]->setPosition(ccp(10, 70));

    m_pTextField[LOGIN_UI_ENUM_ID]->setPosition(ccp(50, 110));
    m_pTextField[LOGIN_UI_ENUM_PW]->setPosition(ccp(50, 70));
    
    CCMenuItemImage *pBtLogin = CCMenuItemImage::create("Button/Login-Login.png", "Button/Login-Login.png", pTarget, loginSelector);
    
    pBtLogin->setPosition(ccp(150,10));
    pBtLogin->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage *pBtJoin = CCMenuItemImage::create("Button/Login-SignUp.png", "Button/Login-SignUp.png", pTarget, joinSelector);
    
    pBtJoin->setPosition(ccp(230,10));
    pBtJoin->setAnchorPoint(ccp(0,0));
        
    m_pMenu = CCMenu::create(pBtLogin, pBtJoin, NULL);
    m_pMenu->setAnchorPoint(ccp(0,0));
    m_pMenu->setPosition(ccp(0,0));
    
    addChild(m_pMenu);
    
    setTouchEnabled(true);
    
    setContentSize(m_pBackGround->getContentSize());
}

LoginUI::~LoginUI()
{
    removeAllChildrenWithCleanup(true);
    
    for(int i=0; i<LOGIN_UI_ENUM_NUM; ++i)
    {
        SAFE_DELETE(m_pTextField[i]);
        SAFE_DELETE(m_pLabel[i]);
    }
    
    SAFE_DELETE(m_pBackGround);
}

void LoginUI::ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent)
{
    TextFieldKeyBoardOn(this, pTouches, m_pTextField, LOGIN_UI_ENUM_NUM);
}

const char* LoginUI::GetString(LOGIN_UI_ENUM e)
{
    return m_pTextField[e]->getString();
}