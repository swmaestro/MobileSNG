//
//  LoginUI.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 21..
//
//

#pragma once

#include "cocos2d.h"

#define LOGIN_UI_FONT_NAME "MarkerFelt"
#define LOGIN_UI_TEXTFILED_SIZE CCSizeMake(200, 25)

enum LOGIN_UI_ENUM {
    LOGIN_UI_ENUM_ID,
    LOGIN_UI_ENUM_PW,
    LOGIN_UI_ENUM_NUM
    };

class LoginUI : public cocos2d::CCLayer, private cocos2d::CCTextFieldDelegate
{
private:
    cocos2d::CCSprite           *m_pBackGround;
    cocos2d::CCTextFieldTTF     *m_pTextField[LOGIN_UI_ENUM_NUM];
    cocos2d::CCLabelTTF         *m_pLabel[LOGIN_UI_ENUM_NUM];
    cocos2d::CCMenu             *m_pMenu;
    
public:
    LoginUI(cocos2d::SEL_MenuHandler loginSelector, cocos2d::SEL_MenuHandler joinSelector, cocos2d::CCObject *pTarget, int fontSize = 20);
    ~LoginUI();
    
private:
    void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    
public:
    const char* GetString(LOGIN_UI_ENUM e);
};