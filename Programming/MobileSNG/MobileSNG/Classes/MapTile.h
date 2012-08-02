//
//  MapTile.h
//  MobileSNG
//
//  Created by 강 건우 on 12. 7. 25..
//  Copyright (c) 2012년 code.athei@gmail.com. All rights reserved.
//

#ifndef MobileSNG_MapTile_h
#define MobileSNG_MapTile_h

#include "cocos2d.h"

class MapTile : public cocos2d::CCLayer
{
private:
    cocos2d::CCSprite   * m_pSpr;
    //Object            * m_pObj;
    
public:
    static int width, height;
    
    MapTile();
    ~MapTile();
    
    virtual bool init();
};

#endif
