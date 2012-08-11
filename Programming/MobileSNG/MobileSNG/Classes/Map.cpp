//
//  Map.cpp
//  MobileSNG
//
//  Created by 강 건우 on 12. 7. 24..
//  Copyright (c) 2012년 code.athei@gmail.com. All rights reserved.
//

#include "Map.h"
#include "Allocator.h"

#include "Shop.h"
#include "SceneGame.h"

using namespace cocos2d;

int Map::width = 480 * 4;
int Map::height = 320 * 4;
int Map::tileWidth = 100;
int Map::tileHeight = 60;

Map::Map() : m_pTile(NULL), m_pAllocator(NULL), m_width(0), m_isDragging(false), m_isScaling(false)
{
    
}

Map::~Map()
{
    removeAllChildrenWithCleanup(true);
    SAFE_DELETE(m_pAllocator);
}

void Map::_initTile()
{
    if (m_pTile)
        removeChild(m_pTile, true);
 
    m_pTile = CCLayer::create();
    
    for (int i = 0; i < m_width; ++i)
        for (int j = 0; j < m_width; ++j)
        {
            CCLayer * tile = CCLayer::create();
            tile->setAnchorPoint(ccp(0.5, 0.5));
            tile->setPosition(ccp((i + j - m_width + 1) * tileWidth / 2, (j - i) * tileHeight / 2));
            
            CCSprite * spr = CCSprite::create("Tile.png");
            spr->setAnchorPoint(ccp(0.5, 0.5));
            
            tile->addChild(spr, 0, 0);
            m_pTile->addChild(tile, i - j + m_width, i * m_width + j);
        }
    
    addChild(m_pTile, 1);
}

int Map::_cursorXY(CCPoint cur)
{
    float scale = getScale();
    
    CCPoint o = ccpSub(getPosition(), ccp((scale - 1) * 240 + m_width / 2 * scale * tileWidth, (scale - 1) * 160));
    
    cur = ccpSub(cur, o);
    cur = ccpMult(cur, 1 / scale);
    
    cur.x /= tileWidth / 2;
    cur.y /= tileHeight / 2;
    CCPoint t = ccp((cur.x - cur.y) / 2, (cur.x + cur.y) / 2);
    
    int x = round(t.x);
    int y = round(t.y);
    
    if (x < 0 || y < 0 || x >= m_width || y >= m_width)
        return -1;
    
    return x * m_width + y;
}

void Map::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent)
{
    if (m_isAllocating)
    {
        CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
        
        CCPoint p = pTouch->locationInView();
        p = CCDirector::sharedDirector()->convertToGL(p);
    
        int t = _cursorXY(p);
        
        if (t < 0)
            return;
        
        m_pAllocator->TouchesBegin(t / m_width, t % m_width);
    }
}

void Map::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (pTouches->count() == 1)
    {
        CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
    
        CCPoint p = pTouch->locationInView();
        p = CCDirector::sharedDirector()->convertToGL(p);
    
        int x = p.x - m_touch[0].x;
        int y = p.y - m_touch[0].y;
    
        if (!m_isDragging && x * x + y * y > 10)
        {
            m_isDragging = true;
            m_isScaling = false;
            m_touch[0] = p;
            
            if (m_isAllocating)
                m_pAllocator->TouchesMove();
        }
        else if (m_isDragging)
        {
            CCPoint o = getPosition();
            CCPoint m = ccp(x, y);
            setPosition(filtPosition(ccpAdd(o, m)));
            m_touch[0] = p;
        }
    }
    else 
    {
        if (m_isAllocating)
            m_pAllocator->TouchesMove();
        
        CCSetIterator i = pTouches->begin();
        
        CCTouch * t1 = static_cast<CCTouch *>(*i++);
        CCTouch * t2 = static_cast<CCTouch *>(*i);
        
        CCPoint p;
        
        p = t1->locationInView();
        p = CCDirector::sharedDirector()->convertToGL(p);
        CCPoint p1 = p;
        
        p = t2->locationInView();
        p = CCDirector::sharedDirector()->convertToGL(p);
        CCPoint p2 = p;
        
        if (m_isScaling)
        {
            float dis1 = ccpDistance(m_touch[0], m_touch[1]);
            float dis2 = ccpDistance(p1, p2);
        
            float s = getScale();
            float a = dis2 / dis1;
            
            float scale = filtScale(s * a);
            
            setScale(scale);
            
            CCPoint p = getPosition();
            CCSize ws = CCDirector::sharedDirector()->getWinSize();
                    
            CCPoint t = ccpMidpoint(m_touch[0], m_touch[1]);
            
            t = ccpSub(t, ccp(ws.width / 2, ws.height / 2));
            t = ccpSub(t, ccpMult(t, scale / s));
            
            p = ccpMult(p, scale / s);
            p = ccpAdd(p, t);
            
            setPosition(filtPosition(p));
        }
        
        m_isScaling = true;
        m_isDragging = false;
        
        m_touch[0] = p1;
        m_touch[1] = p2;
    }
}

void Map::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if (!m_isDragging && !m_isScaling)
    {
        if (m_isAllocating)
        {
            CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
        
            CCPoint p = pTouch->locationInView();
            p = CCDirector::sharedDirector()->convertToGL(p);
        
            int t = _cursorXY(p);
        
            if (t < 0)
                return;
        
            m_pAllocator->TouchesEnd(t / m_width, t % m_width);
        }
    }
    
    m_isDragging = false;
    m_isScaling = false;
}

bool Map::init()
{
    if (!CCLayer::init())
        return false;
    
    m_width = 5;
    _initTile();
    
    CCSprite * bg = CCSprite::create("Background.png");
    bg->setAnchorPoint(ccp(0.5, 0.5));
    bg->setPosition(ccp(0, 0));
    bg->setScale(4);
    addChild(bg, 0);
    
    m_pAllocator = new Allocator();
    m_pAllocator->setAnchorPoint(ccp(0.5, 0.5));
    m_pAllocator->setPosition(ccp(0, 0));
    m_pAllocator->setVisible(false);
    addChild(m_pAllocator, 2);
    
    return true;
}

float Map::filtScale(float scale)
{
    if (scale < 0.5) scale = 0.5;
    if (scale > 1) scale = 1;
    
    return scale;
}

CCPoint Map::filtPosition(CCPoint pos)
{
    float scale = getScale();
    
    if (pos.x > -240 + (Map::width / 2 + 240) * scale)
        pos.x = -240 + (Map::width / 2 + 240) * scale;
    if (pos.y > -160 + (Map::height / 2 + 160) * scale)
        pos.y = -160 + (Map::height / 2 + 160) * scale;
    
    if (pos.x < 240 - (Map::width / 2 - 240) * scale)
        pos.x = 240 - (Map::width / 2 - 240) * scale;
    if (pos.y < 160 - (Map::height / 2 - 160) * scale)
        pos.y = 160 - (Map::height / 2 - 160) * scale;
    
    return pos;
}

void Map::beginEdit(MapMgr * mapMgr)
{
    
}

void Map::beginEdit(MapMgr * mapMgr, int type, int id)
{
    m_isAllocating = true;
    
    m_pAllocator->init(mapMgr, m_width, type, id);
    m_pAllocator->setVisible(true);
}

void Map::endEdit(bool apply)
{
    if (apply)
    {
        if (m_isAllocating)
        {
            m_pAllocator->Apply();

            for (int i = 0; i < m_pAllocator->m_vec.size(); ++i)
            {
                CCNode * tile = m_pTile->getChildByTag(m_pAllocator->m_vec[i]);
                
                if (tile == NULL)
                    continue;
                
                char temp[30];
                if (m_pAllocator->m_type == OBJ_FARM)
                    sprintf(temp, "Farm.png");
                else
                    sprintf(temp, "%s/01.png", tempString[m_pAllocator->m_type][m_pAllocator->m_id]);
                
                CCSprite * spr = CCSprite::create(temp);
                
                switch (m_pAllocator->m_type)
                {
                    case OBJ_FARM:
                        spr->setAnchorPoint(ccp(0.5, 0.5));
                        tile->addChild(spr, 1, 1);
                        break;
                        
                    case OBJ_BUILDING:
                        spr->setAnchorPoint(ccp(0.5, 0.3));
                        tile->addChild(spr, 1, 1);
                        break;
                        
                    case OBJ_CROP:
                        spr->setAnchorPoint(ccp(0.5, 0.3));
                        tile->addChild(spr, 2, 2);
                }
            }
        }
    }
    
    m_pAllocator->removeAllChildrenWithCleanup(true);
    m_pAllocator->setVisible(false);
    
    m_isAllocating = false;
}