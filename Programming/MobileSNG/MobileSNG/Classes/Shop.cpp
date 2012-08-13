//
//  Shop.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 23..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Shop.h"
#include "GameScene.h"

using namespace cocos2d;

Shop::Shop() : m_pTabTex(NULL), m_pTabSelTex(NULL), m_isDragging(false), m_selected(0)
{
    for (int i = 0; i < OBJ_MAX; ++i)
        m_pTab[i] = NULL;
}

Shop::~Shop()
{
    removeAllChildrenWithCleanup(true);
    
    for (int i = 0; i < OBJ_MAX; ++i)
    {
        SAFE_DELETE(m_pTab[i]);
        SAFE_DELETE(m_pItem[i]);
    }
}

bool Shop::init(GameScene * scene)
{  
    if (!CCLayer::init())
        return false;

    m_pScene = scene;
    
    CCSprite * pBG = CCSprite::create("Shop-Background.png");
    pBG->setAnchorPoint(ccp(0, 0));
    pBG->setPosition(ccp(0, 0));
    pBG->setOpacity(150);
    addChild(pBG, 0);
    
    CCSprite * pLine = CCSprite::create("Shop-Line.png");
    pLine->setAnchorPoint(ccp(0, 0));
    pLine->setPosition(ccp(0, 0));
    addChild(pLine, 0);
    
    m_pTabTex = CCTextureCache::sharedTextureCache()->addImage("Shop-Tab.png");
    m_pTabSelTex = CCTextureCache::sharedTextureCache()->addImage("Shop-TabSelected.png");
    
    for (int i = 0; i < OBJ_MAX; ++i)
    {
        m_pTab[i] = new CCSprite();
        m_pTab[i]->initWithTexture(m_pTabTex);
        m_pTab[i]->setAnchorPoint(ccp(0.5, 0.5));
        m_pTab[i]->setPosition(ccp(10 + (i + 0.5) * 100, 295));
        addChild(m_pTab[i], 1);
        
        m_pItem[i] = new CCLayer();
        m_pItem[i]->init();
        m_count[i] = 0;
    }

//120809 CA : Must be removed
    
    addItem(OBJ_CROP, "", "CandyCane/CandyCane.png", 0, 0, 0, 0, 0);
    addItem(OBJ_CROP, "", "MushMallow/MushMallow.png", 0, 0, 0, 0, 0);
    addItem(OBJ_CROP, "", "JellyBean/JellyBean.png", 0, 0, 0, 0, 0);
    
    addItem(OBJ_BUILDING, "", "Farm/Farm.png", 0, 0, 0, 0, 0);
    addItem(OBJ_BUILDING, "", "HauntedHouse/HauntedHouse.png", 0, 0, 0, 0, 0);
    
/////////////////////////////
    
    _select(OBJ_CROP);
    
    return true;
}

void Shop::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
    
    m_touch = pTouch->locationInView();
    m_touch = CCDirector::sharedDirector()->convertToGL(m_touch);
}

void Shop::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
    
    CCPoint p = pTouch->locationInView();
    p = CCDirector::sharedDirector()->convertToGL(p);
    
    int x = p.x - m_touch.x;
    
    if (!m_isDragging && x * x > 10)
    {
        if (m_touch.y >= 275)
            return;
        
        m_isDragging = true;
    }
    else if (m_isDragging)
    {
        float t = m_pItem[m_selected]->getPositionX() + x;
   
        if (t < -m_count[m_selected] * 200 + 480)
            t = -m_count[m_selected] * 200 + 480;
        if (t > 0)
            t = 0;
        
        m_pItem[m_selected]->setPositionX(t);
        
        m_touch = p;
    }
}

void Shop::_select(int i)
{
    m_pTab[m_selected]->setTexture(m_pTabTex);
    m_pTab[i]->setTexture(m_pTabSelTex);
    
    m_pTab[m_selected]->setPositionY(295);
    m_pTab[i]->setPositionY(288);
    
    removeChild(m_pItem[m_selected], false);
    addChild(m_pItem[i], 1);

    m_selected = i;
}

void Shop::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if (!m_isDragging)
    {
        if (m_touch.y >= 275 && m_touch.x >= 10 && m_touch.x < 10 + OBJ_MAX * 100)
        {
            int i = (m_touch.x - 10) / 100;
            _select(i);
        }
        else if (m_touch.y < 275)
        {
            int i = (m_touch.x - m_pItem[m_selected]->getPositionX()) / 200;
            
            if (i >= m_count[m_selected])
                return;
            
            if (m_selected == OBJ_BUILDING && i == 0)
                m_pScene->alloc(OBJ_FARM, 0);
            else
            {
                if (m_selected == OBJ_BUILDING)
                    --i;
                m_pScene->alloc(m_selected, i);
            }
        }
    }

    m_isDragging = false;
}

void Shop::addItem(int tabNum, const char *name, const char *imgPath,
                   int costSweet, int costFear, int time, int rewardSweet, int rewardFear)
{
    CCSprite * spr = CCSprite::create(imgPath);
    spr->setAnchorPoint(ccp(0.5, 0.5));
    spr->setPosition(ccp((m_count[tabNum]++ + 0.5) * 200, 160));
    
    m_pItem[tabNum]->addChild(spr);
}