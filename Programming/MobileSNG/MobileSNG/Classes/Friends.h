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

class Friends : public cocos2d::CCLayer
{
private:
    GameSystem * m_pSystem;
    Social * m_pSocial;
    
public:
    Friends(GameSystem * system);
    ~Friends();
    
    bool init();
    
    void refresh();
};

#endif /* defined(__MobileSNG__Friends__) */

