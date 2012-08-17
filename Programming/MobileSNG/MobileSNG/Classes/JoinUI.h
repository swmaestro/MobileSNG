//
//  JoinUI.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 13..
//
//

#include "cocos2d.h"

#define JOIN_UI_FONT_NAME "MarkerFelt"

enum JOIN_UI_ENUM
{
    JOIN_UI_ENUM_ID,
    JOIN_UI_ENUM_PW,
    JOIN_UI_ENUM_PHONE,
    JOIN_UI_ENUM_NUM
};

class JoinUI : public cocos2d::CCLayer, public cocos2d::CCTextFieldDelegate
{
private:
    cocos2d::CCSprite           *m_pBackGround;
    cocos2d::CCLabelTTF         *m_pLabel[JOIN_UI_ENUM_NUM];
    cocos2d::CCTextFieldTTF     *m_pTextField[JOIN_UI_ENUM_NUM];

    cocos2d::CCMenu             *m_pMenu;
    
public:
    JoinUI(const char* strBackGroundPath, cocos2d::CCNode *pTarget, cocos2d::SEL_MenuHandler JoinSelector, cocos2d::SEL_MenuHandler CancelSelector, cocos2d::SEL_MenuHandler OverlabSelector,int fontSize = 20);
    ~JoinUI();
    
public:
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF * sender);
    
    virtual void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    
public:
    void setEmptyTextField(JOIN_UI_ENUM e);
    
public:
    const char* GetContext(JOIN_UI_ENUM e);
};