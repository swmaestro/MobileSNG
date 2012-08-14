//
//  UIMgr.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 23..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "JoinUI.h"

//CA Edit 120728 Move Shop to Game Scene and Change UI System
//CA Edit 120729 Change UI System

#define UI_FUNC(x) menu_selector(GameScene::x)

enum
{
    UI_MAP, UI_EDIT, UI_SHOP, UI_MAX
};

class GameScene;

class UIMgr : public cocos2d::CCNode
{
private:
    cocos2d::CCArray           * m_pUI[UI_MAX];
    cocos2d::CCMenu            * m_pMenu;
    
    GameScene                  * m_pScene;
    
public:
    UIMgr(GameScene * scene);
    ~UIMgr();
    
    bool AppendUI(int uiNum, cocos2d::CCPoint pos,
                  const char * normalImage, const char * selectedImage,
                  cocos2d::SEL_MenuHandler handler);
    
    bool ChangeUI(int uiNum);

public:
//void join(cocos2d::CCObject *pSender);
//void Cancel(cocos2d::CCObject *pSender);
};