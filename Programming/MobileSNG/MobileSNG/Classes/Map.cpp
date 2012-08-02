//
//  Map.cpp
//  MobileSNG
//
//  Created by 강 건우 on 12. 7. 24..
//  Copyright (c) 2012년 code.athei@gmail.com. All rights reserved.
//

#include "Map.h"
#include "MapTile.h"

using namespace cocos2d;

Map::Map() : m_arrTile(NULL), m_width(0), m_isDragging(false), m_isScaling(false)
{
    
}

Map::~Map()
{
    removeAllChildrenWithCleanup(true);
    _release(m_arrTile, m_width);
}

MapTile *** Map::_create(int width)
{
    MapTile *** t = NULL;
    
    t = new MapTile ** [width];
    for (int i = 0; i < width; ++i)
    {
        t[i] = new MapTile * [width];
        
        for (int j = 0; j < width; ++j)
        {
            t[i][j] = new MapTile();
            t[i][j]->init();
        }
    }
    
    return t;
}

void Map::_release(MapTile *** tile, int width)
{
    if (tile)
    {
        for (int i = 0; i < width; ++i)
            if (tile[i])
            {
                for (int j = 0; j < width; ++j)
                    if (tile[i][j])
                    {
                        delete tile[i][j];
                        tile[i][j] = NULL;
                    }
            
                delete [] tile[i];
                tile[i] = NULL;
            }
        
        delete [] tile;
        tile = NULL;
    }
}

void Map::_addTile()
{
    if (m_arrTile)
        for (int i = 0; i < m_width; ++i)
            if (m_arrTile[i])
                for (int j = 0; j < m_width; ++j)
                    if (m_arrTile[i][j])
                    {
                        m_arrTile[i][j]->setAnchorPoint(ccp(0, 0));
                        m_arrTile[i][j]->setPosition(
                            ccp((i + j - m_width + 1) * MapTile::width / 2, (j - i) * MapTile::height / 2));
                        addChild(m_arrTile[i][j], 1);
                    }
}

void Map::_removeTile()
{
    if (m_arrTile)
        for (int i = 0; i < m_width; ++i)
            if (m_arrTile[i])
                for (int j = 0; j < m_width; ++j)
                    if (m_arrTile[i][j])
                        removeChild(m_arrTile[i][j], true);
}

#define ABS(x) ((x) < 0 ? -(x) : (x)) 

int Map::_cursorXY(CCPoint cur)
{
    CCPoint o = getPosition();
    CCRect rc = boundingBox();
    float scale = getScale();
    
    o.x -= (scale - 1) * rc.size.width / scale / 2 + m_width / 2 * scale * MapTile::width;
    o.y -= (scale - 1) * rc.size.height / scale / 2;
    
    /*
    for (int i = 0; i < m_width; ++i)
        for (int j = 0; j < m_width; ++j)
        {
            CCPoint t = m_arrTile[i][j]->getPosition();
            
            t.x = cur.x - o.x - t.x * scale;
            t.y = cur.y - o.y - t.y * scale;
            
            if (ABS(t.x / (MapTile::width * scale / 2)) + ABS(t.y / (MapTile::height * scale / 2)) < 1)
                return i * m_width + j;
        }
    
    return -1;
     */
    
    cur = ccpSub(cur, o);
    cur.x /= MapTile::width / 2 * scale;
    cur.y /= MapTile::height / 2 * scale;
    CCPoint t = ccp((cur.x - cur.y) / 2, (cur.x + cur.y) / 2);
    
    int x = round(t.x);
    int y = round(t.y);
    
    if (x < 0 || y < 0 || x >= m_width || y >= m_width)
        return -1;
    
    return x * m_width + y;
}

void Map::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent)
{
    //
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
            
            if (a > 1.1) a = 1.1;
            if (a < 0.9) a = 0.9;
        
            float scale = filtScale(s * a);
            
            setScale(scale);
            
            CCPoint p = getPosition();
            CCRect rc = boundingBox();
            CCSize ws = CCDirector::sharedDirector()->getWinSize();
                    
            CCPoint t = ccpMidpoint(m_touch[0], m_touch[1]);
            
            t = ccpSub(t, ccp(ws.width / 2, ws.height / 2));
            t = ccpSub(t, ccpMult(t, scale / s));
            
            p = ccpMult(p, scale / s);
            p = ccpAdd(p, t);
            
            //p.x -= (scale - 1) * rc.size.width / scale / 2;
            //p.y -= (scale - 1) * rc.size.height / scale / 2;
            
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
        CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
        
        CCPoint p = pTouch->locationInView();
        p = CCDirector::sharedDirector()->convertToGL(p);
        
        int t = _cursorXY(p);
        
        if (t < 0)
            return;
        
        int x = t / m_width;
        int y = t % m_width;
        
        CCSprite * spr = CCSprite::create("Farm.png");
        spr->setAnchorPoint(ccp(0.5, 0.5));
        spr->setPosition(ccp(0, 0));
        m_arrTile[x][y]->addChild(spr);
    }
    
    m_isDragging = false;
    m_isScaling = false;
}

bool Map::init()
{
    if (!CCLayer::init())
        return false;
    
    if (m_arrTile)
    {
        _removeTile();
        _release(m_arrTile, m_width);
    }
    
    m_width = 5;
    m_arrTile = _create(m_width);
    _addTile();
    
    m_pBackGround = CCSprite::create("Background.png");
    m_pBackGround->setAnchorPoint(ccp(0.5, 0.5));
    m_pBackGround->setPosition(ccp(0, 0));
    addChild(m_pBackGround, 0);
    
    return true;
}

float Map::filtScale(float scale)
{
    if (scale < 0.75) scale = 0.75;
    if (scale > 1.75) scale = 1.75;
    
    return scale;
}

CCPoint Map::filtPosition(CCPoint pos)
{
    CCRect rc = boundingBox();
    float scale = getScale();
    
    if (pos.x > rc.size.width + (scale - 1) / scale * rc.size.width / 2)
        pos.x = rc.size.width + (scale - 1) / scale * rc.size.width / 2;
    if (pos.y > rc.size.height + (scale - 1) / scale * rc.size.height / 2)
        pos.y = rc.size.height + (scale - 1) / scale * rc.size.height / 2;
    
    if (pos.x < (1 - scale) / scale * rc.size.width / 2)
        pos.x = (1 - scale) / scale * rc.size.width / 2;
    if (pos.y < (1 - scale) / scale * rc.size.height / 2)
        pos.y = (1 - scale) / scale * rc.size.height / 2;
    
    /*
    if (pos.x > rc.size.width + (scale - 1) * wsize.width / 2)
        pos.x = rc.size.width + (scale - 1) * wsize.width / 2;
    if (pos.y > rc.size.height + (scale - 1) * wsize.height / 2)
        pos.y = rc.size.height + (scale - 1) * wsize.height / 2;
    
    if (pos.x < (1 - scale) * wsize.width / 2)
        pos.x = (1 - scale) * wsize.width / 2;
    if (pos.y < (1 - scale) * wsize.height / 2)
        pos.y = (1 - scale) * wsize.height / 2;
    */
    return pos;
}