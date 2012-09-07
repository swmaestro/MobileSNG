//
//  Friends.h
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 23..
//
//

#ifndef __MobileSNG__Friends__
#define __MobileSNG__Friends__

#include "cocos2d.h"
#include "GameSystem.h"
#include "Social.h"

class Friends : public cocos2d::CCLayer, public cocos2d::CCTextFieldDelegate
{
private:
    GameSystem * m_pSystem;
    Social * m_pSocial;
    Network * m_pNetwork;
    
    cocos2d::CCLayer * m_pMovable;
    cocos2d::CCTextFieldTTF * m_pTextField;
    
public:
    Friends(GameSystem * system, Network *pNetwork);
    ~Friends();
    
    bool init();
    
    void addFriendList(std::string name, int num = 0);
    
    void refresh();
    void refreshBySearch(std::string name);
    
    void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    bool onTextFieldInsertText(cocos2d::CCTextFieldTTF * sender, const char * text, int nLen);
};

#endif /* defined(__MobileSNG__Friends__) */

