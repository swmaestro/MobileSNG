//
//  UIMgr.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 23..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "UIMgr.h"
#include "GameScene.h"

using namespace cocos2d;

UIMgr::UIMgr(GameScene * scene) : m_pScene(scene)
{
    for (int i = 0; i < UI_MAX; ++i)
    {
        m_pUI[i] = new CCArray;
        m_pUI[i]->init();
    }
    
    m_pMenu = NULL;
}

UIMgr::~UIMgr()
{
    m_pMenu->removeAllChildrenWithCleanup(true);
    removeAllChildrenWithCleanup(true);
    
    for (int i = 0; i < UI_MAX; ++i)
        SAFE_DELETE(m_pUI[i]);
}

bool UIMgr::AppendUI(int uiNum, CCPoint pos, const char *normalImage, const char *selectedImage, SEL_MenuHandler handler)
{
    if (uiNum < 0 || uiNum >= UI_MAX)
        return false;
    
    CCMenuItemImage * b = NULL;
    b = CCMenuItemImage::create(normalImage, selectedImage, NULL, m_pScene, handler);
    
    if (b == NULL)
        return false;
    
    b->setPosition(pos);
    m_pUI[uiNum]->addObject(b);
    
    return true;
}

bool UIMgr::ChangeUI(int uiNum)
{
    if (uiNum < 0 || uiNum >= UI_MAX)
        return false;
    
    if (m_pMenu)
    {
        removeChild(m_pMenu, true);
        m_pMenu = NULL;
    }

    if (!m_pUI[uiNum] || m_pUI[uiNum]->count() <= 0)
        return false;
    
    m_pMenu = CCMenu::create(m_pUI[uiNum]);
    addChild(m_pMenu);
    
    return true;
}