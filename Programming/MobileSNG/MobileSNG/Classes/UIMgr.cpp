//
//  UIMgr.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 23..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "UIMgr.h"

using namespace cocos2d;

UIMgr::UIMgr()
{
//    m_pMapUI            = NULL;
//    m_pShopUI           = NULL;
    
    for (int i = 0; i < UI_MAX; ++i)
    {
        m_pUI[i] =  new CCArray;
        m_pUI[i]->init();
    }
    
    m_pMenu             = NULL;
}

UIMgr::~UIMgr()
{
    m_pMenu->removeAllChildrenWithCleanup(true);
    removeAllChildrenWithCleanup(true);
    
//    SAFE_DELETE(m_pMapUI);
//    SAFE_DELETE(m_pShopUI);
    
    for (int i = 0; i < UI_MAX; ++i)
        SAFE_DELETE(m_pUI[i]);
}

bool UIMgr::AppendUI(int uiNum, const char *normalImage, const char *selectedImage, CCPoint pos, 
                     CCLayer *pScene, SEL_MenuHandler handler)
{
    if (uiNum < 0 || uiNum >= UI_MAX)
        return false;
    
    CCMenuItemImage * b = NULL;
    b = CCMenuItemImage::create(normalImage, selectedImage, NULL, pScene, handler);
    
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

/*
bool UIMgr::CreateUI(cocos2d::CCScene *pScene, cocos2d::SEL_MenuHandler shopFunc, 
                     cocos2d::SEL_MenuHandler editFunc, cocos2d::SEL_MenuHandler flatFunc,
                     cocos2d::SEL_MenuHandler shopCloseFunc)
{
    CCMenuItemImage * b = NULL;
    
    ////////Map UI////////
    
    m_pMapUI = new CCArray;
    
    if ((b = CCMenuItemImage::create("Edit.png", "Edit.png", NULL, pScene, editFunc)) == NULL)
        return false;
    b->setPosition(160, -120);
    m_pMapUI->addObject(b);

    if ((b = CCMenuItemImage::create("Flat.png", "Flat.png", NULL, pScene, flatFunc)) == NULL)
        return false;
    b->setPosition(210, 120);
    m_pMapUI->addObject(b);

    if ((b = CCMenuItemImage::create("Friends.png", "Friends.png", NULL, this, 
                                     menu_selector(UIMgr::_buttonFriend))) == NULL)
        return false;
    b->setPosition(-210, -120);
    m_pMapUI->addObject(b);

    if ((b = CCMenuItemImage::create("Shop.png", "Shop.png", NULL, pScene, shopFunc)) == NULL)
        return false;
    b->setPosition(210, -120);
    m_pMapUI->addObject(b);
    
    
    ////////Shop UI////////

    m_pShopUI = new CCArray;
    
    if ((b = CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", NULL, pScene, shopCloseFunc)) == NULL)
        return false;
    b->setPosition(210, -120);
    m_pShopUI->addObject(b);
    
    return true;
}

void UIMgr::_buttonFriend(CCObject *pSender)
{
    CCLOG(__FUNCTION__);
}

void UIMgr::MapUI()
{
    removeChild(m_pMenu, true);
    m_pMenu = CCMenu::create(m_pMapUI);
    addChild(m_pMenu);
}

void UIMgr::ShopUI()
{
    removeChild(m_pMenu, true);
    m_pMenu = CCMenu::create(m_pShopUI);
    addChild(m_pMenu);
}*/

/*void UIMgr::addUI()
{
    m_pScene->addChild(m_pScene);
    m_pScene->addChild(m_pMenu);
    m_pScene->addChild(m_pShop);
    //SHOP MOVE 
}*/