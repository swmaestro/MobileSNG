//
//  PlayerMap.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 9. 9..
//
//

#include "PlayerMap.h"
#include "Allocator.h"
#include "Utility.h"

using namespace cocos2d;

PlayerMap::PlayerMap(int & width) : Map(width)
{
    
}

PlayerMap::~PlayerMap()
{
    delete m_pAllocator;
}

bool PlayerMap::init(GameSystem * system)
{
    if (!Map::init(system))
        return false;
    
    m_pAllocator = new Allocator(m_pTile, m_width);
    return true;
}

void PlayerMap::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent)
{
    CCSetIterator i = pTouches->begin();
    
    while (m_touchCnt < 1 && i != pTouches->end())
        m_touchID[++m_touchCnt] = static_cast<CCTouch *>(*i++)->getID();
    
    if (m_isAllocating)
    {
        if (m_touchCnt == 0 && pTouches->count() == 1)
        {
            CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
            
            CCPoint p = pTouch->locationInView();
            p = CCDirector::sharedDirector()->convertToGL(p);
            
            int t = _cursorXY(p);
            int x = LOWORD(t), y = HIWORD(t);
            
            if (x < -m_width / 2 || x > m_width / 2 || y < -m_width / 2 || y > m_width / 2)
                return;
            
            m_pAllocator->TouchesBegin(x, y);
        }
        else
            m_pAllocator->TouchesMove();
    }
}

void PlayerMap::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (m_touchCnt == 0 && pTouches->count() == 1 && !m_isScaling)
    {
        CCTouch * pTouch = static_cast<CCTouch *>(*pTouches->begin());
        
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
            filtPosition(ccpAdd(o, m));
            m_touch[0] = p;
        }
    }
    else if (m_touchCnt == 1 && pTouches->count() == 2)
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
    else
    {
        m_isDragging = false;
        m_isScaling = false;
    }
}

void PlayerMap::beginEdit()
{
    
}

void PlayerMap::beginEdit(int type, int id)
{
    m_isAllocating = true;
    
    m_pAllocator->init(m_pSystem, type, id);
}

void PlayerMap::endEdit(bool apply)
{
    if (m_isAllocating)
    {
        if (apply)
            m_pAllocator->Apply();
        else
            m_pAllocator->Cancel();
        
        m_isAllocating = false;
        m_pAllocator->Clear();
    }
    else if (m_isEditing)
    {
        if (apply)
        {
            
        }
    }
}