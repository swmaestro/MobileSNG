//
//  Shop.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 23..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "Shop.h"

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

bool Shop::init()
{  
    if (!CCLayer::init())
        return false;
    
    CCSprite * pBG = CCSprite::create("Shop-Background.png");
    pBG->setAnchorPoint(ccp(0, 0));
    pBG->setPosition(ccp(0, 0));
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
        m_pTab[i]->addChild(m_pItem[i]);
    }
    
    _select(0);
    
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
    int y = p.y - m_touch.y;
    
    if (!m_isDragging && x * x + y * y > 10)
    {
        if (m_touch.y >= 275)
            return;
        
        m_isDragging = true;
    }
    else if (m_isDragging)
    {
        CCLog("DRAG");
        
    }
}

void Shop::_select(int i)
{
    m_pTab[m_selected]->setTexture(m_pTabTex);
    m_pTab[i]->setTexture(m_pTabSelTex);
    
    m_pTab[m_selected]->setPositionY(295);
    m_pTab[i]->setPositionY(288);
    
//    reorderChild(m_pTab[m_selected], 1);
//    reorderChild(m_pTab[i], 3);

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
    }

    m_isDragging = false;
}