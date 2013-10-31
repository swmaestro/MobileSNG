//
//  Talkbox.h
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 22..
//
//

#ifndef __MobileSNG__Talkbox__
#define __MobileSNG__Talkbox__

#include "cocos2d.h"
#include <string>

enum TALKBOX_TYPE
{
    TALKBOX_TYPE_FARM,
    TALKBOX_TYPE_BUILDING,
    TALKBOX_TYPE_UNKNOWN
};

class Talkbox : public cocos2d::CCLayer
{
public:
    LAYER_CREATE_FUNC(Talkbox);
    
    Talkbox();
    ~Talkbox();
    
    virtual bool init();
    
    void SetContent(cocos2d::CCPoint pos, TALKBOX_TYPE type);
    
    bool Touch(cocos2d::CCTouch * touch);
    cocos2d::CCPoint GetPos() { return m_pos; }
    
private:
    cocos2d::CCPoint m_pos;
    cocos2d::CCLayer * m_pScale;
    cocos2d::CCSprite * m_pTalkbox, * m_pRemove;
    cocos2d::CCLabelTTF * m_pText[3];
    
    std::string m_content;
};

#endif /* defined(__MobileSNG__Talkbox__) */
  