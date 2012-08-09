//
//  Shop.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 23..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cocos2d.h"

enum
{
    OBJ_CROP, OBJ_BUILDING, OBJ_MAX
};

class Shop : public cocos2d::CCLayer
{
private:
    cocos2d::CCSprite * m_pTab[OBJ_MAX];
    cocos2d::CCLayer * m_pItem[OBJ_MAX];
    int m_count[OBJ_MAX];
    
    cocos2d::CCTexture2D * m_pTabTex, * m_pTabSelTex;
    
    bool m_isDragging;
    cocos2d::CCPoint m_touch;
    
    int m_selected;
    void _select(int i);
    
public:
    Shop();
    ~Shop();
    
    bool init();
    
//  void addItem(int tabNum, int objNum);
    void addItem(int tabNum, const char * name, const char * imgPath, int price, int time);
    
    virtual void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
};