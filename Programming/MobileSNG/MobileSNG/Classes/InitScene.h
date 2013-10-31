//
//  InitScene.h
//  MobileSNG
//
//  Created by 건우 강 on 12. 9. 11..
//
//

#ifndef __MobileSNG__InitScene__
#define __MobileSNG__InitScene__

#include "cocos2d.h"

class InitScene : public cocos2d::CCLayer
{
public:
    LAYER_CREATE_FUNC(InitScene);
    
    virtual bool init();
    virtual void onEnter();

    static cocos2d::CCScene* scene();
};

#endif /* defined(__MobileSNG__InitScene__) */
