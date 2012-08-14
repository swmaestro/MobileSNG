//
//  JoinUI.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 13..
//
//

#include "cocos2d.h"

#define JOIN_UI_FONT_NAME "MarkerFelt"

class JoinUI : public cocos2d::CCLayer, public cocos2d::CCTextFieldDelegate
{
private:
    cocos2d::CCSprite           *m_pBackGround;
    cocos2d::CCLabelTTF         *m_pIDLabel;
    cocos2d::CCLabelTTF         *m_pPassWordLabel;
    cocos2d::CCTextFieldTTF     *m_pIDTextField;
    cocos2d::CCTextFieldTTF     *m_pPWTextField;
    
    cocos2d::CCMenu             *m_pMenu;
    
public:
    JoinUI(const char* strBackGroundPath, cocos2d::CCNode *pTarget, cocos2d::SEL_MenuHandler JoinSelector, cocos2d::SEL_MenuHandler CancelSelector,  int fontSize = 20);
    ~JoinUI();
    
public:
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF * sender);
    
    virtual void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    
public:
    const char* GetID();
    const char* GetPassWord();
};