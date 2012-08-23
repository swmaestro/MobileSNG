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

class Talkbox : public cocos2d::CCLayer
{
public:
    LAYER_CREATE_FUNC(Talkbox);
    
    virtual bool init();
    
    void SetContent(std::string content);
    
private:
    cocos2d::CCLabelTTF * m_pText;
    
    std::string m_content;
};

#endif /* defined(__MobileSNG__Talkbox__) */
