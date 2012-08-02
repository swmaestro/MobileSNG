//
//  UIMgr.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 23..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Shop.h"

//CA Edit 120728 Move Shop to Game Scene and Change UI System
//CA Edit 120729 Change UI System

enum
{
    UI_MAP, UI_SHOP, UI_MAX
};

class UIMgr : public cocos2d::CCNode
{
private:
    cocos2d::CCArray           * m_pUI[UI_MAX];
    cocos2d::CCMenu            * m_pMenu;
    
public:
    UIMgr();
    ~UIMgr();
/*
private:
    void _buttonFriend(cocos2d::CCObject *pSender);

public:
    bool CreateUI(cocos2d::CCScene *pScene,
                  cocos2d::SEL_MenuHandler shopFunc,
                  cocos2d::SEL_MenuHandler editFunc, 
                  cocos2d::SEL_MenuHandler flatFunc, 
                  cocos2d::SEL_MenuHandler shopCloseFunc);
 
    void MapUI();
    void ShopUI();
*/    
    
    bool AppendUI(int uiNum, 
                  const char * normalImage, const char * selectedImage, cocos2d::CCPoint pos,
                  cocos2d::CCLayer * pScene, cocos2d::SEL_MenuHandler handler);
    
    bool ChangeUI(int uiNum);
};