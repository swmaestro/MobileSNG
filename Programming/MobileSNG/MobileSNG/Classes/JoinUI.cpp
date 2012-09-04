//
//  JoinUI.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 13..
//
//

#include "JoinUI.h"
#include "Utility.h"
#include <string>

using namespace cocos2d;

JoinUI::JoinUI(CCNode *pTarget, cocos2d::SEL_MenuHandler JoinSelector, cocos2d::SEL_MenuHandler CancelSelector, SEL_MenuHandler RepetitionSelector, int fontSize)
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
        
    CCMenuItemImage *pJoinButton = CCMenuItemImage::create("Button/SignUp-SignUp.png", "Button/SignUp-SignUp.png", pTarget, JoinSelector);
    pJoinButton->setPosition(ccp(110,0));
    pJoinButton->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage *pCancelButton = CCMenuItemImage::create("Button/SignUp-Cancel.png", "Button/SignUp-Cancel.png", pTarget, CancelSelector);
    pCancelButton->setPosition(ccp(170,0));
    pCancelButton->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage *pRepetitionIDButton = CCMenuItemImage::create("Icon.png", "Icon.png", pTarget, RepetitionSelector);
    pRepetitionIDButton->setPosition(ccp(230,0));
    pRepetitionIDButton->setAnchorPoint(ccp(0,0));
    
    m_pMenu = CCMenu::create(pJoinButton, pCancelButton, pRepetitionIDButton, NULL);
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
    if(sender != m_pTextField[JOIN_UI_ENUM_PHONE])      return false;
    if(text[0] == '\n')                                 return false;
    if(('0' <= text[0] && text[0] <= '9') == false)     return true;
    
    std::string phone    = sender->getString();
    int length           = phone.length();
    
    //최대 사이즈 벗어나
    if(length > 12)
        return true;
    
    if(length == 3 || length == 7)
        phone += '-';
    
    if(length == 12 )
    {
        const char phone7 = phone.data()[7];
        const char phone8 = phone.data()[8];
        
        phone[7] = phone8;
        phone[8] = phone7;
    }
    
    sender->setString(phone.data());
    
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

const char* JoinUI::GetID()
{
    return m_pTextField[JOIN_UI_ENUM_ID]->getString();
}

const char* JoinUI::GetPW()
{
    return m_pTextField[JOIN_UI_ENUM_PW]->getString();
}

const char* JoinUI::GetPhone()
{
    std::string phone = m_pTextField[JOIN_UI_ENUM_PHONE]->getString();
    
    phone.erase(phone.begin()+3);
    if(phone.length() == 12)
        phone.erase(phone.begin()+7);
    else
        phone.erase(phone.begin()+6);
    
    return phone.data();
}

void JoinUI::AllClear()
{
    for(int i=0; i<JOIN_UI_ENUM_NUM; ++i)
        m_pTextField[i]->setString("");
}