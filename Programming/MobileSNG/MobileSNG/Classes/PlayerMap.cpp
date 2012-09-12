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
#include "GameSystem.h"
#include "Player.h"
#include "Talkbox.h"
#include "Thread.h"

using namespace cocos2d;

PlayerMap::PlayerMap(int width) : Map(width), m_pAllocator(NULL), m_isAllocating(false), m_isEditing(false)
{
    
}

PlayerMap::~PlayerMap()
{
    delete m_pAllocator;
}

void PlayerMap::update(float dt)
{
    std::vector<ObjectInMap *> object = m_pSystem->GetAllObject();
    std::vector<ObjectInMap *>::iterator i;
    
    for (i = object.begin(); i != object.end(); ++i)
        if ((*i)->UpdateSystem())
        {
            int index = (*i)->GetIndex();
            
            if( (*i)->isConstruct() )
                m_pSystem->buildingConstructCheck(index);
            
            SyncPos(this, *i);
        }
}


bool PlayerMap::init(GameSystem * system)
{
    if (!Map::init(system, NULL))
        return false;
    
    m_pAllocator = new Allocator(m_pTile, m_width, this);
    return true;
}

void PlayerMap::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent)
{
    CCSetIterator i = pTouches->begin();
    
    while (m_touchCnt < 1 && i != pTouches->end())
    {
        m_touchID[++m_touchCnt] = static_cast<CCTouch *>(*i)->getID();
        m_touch[m_touchCnt] = ((CCTouch *)*i++)->locationInView();
        m_touch[m_touchCnt] = CCDirector::sharedDirector()->convertToGL(m_touch[m_touchCnt]);
    }
    
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

void PlayerMap::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if (m_touchCnt == 0 && pTouches->count() == 1 && !m_isDragging && !m_isScaling)
    {
        if(m_isAllocating)
            m_pAllocator->TouchesEnd();
        else
        {
            CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
            
            if (m_pTalkbox->Touch(pTouch))
            {
                CCPoint p = m_pTalkbox->GetPos();
                ObjectInMap * oim = m_pSystem->FindObject(POINT<int>(p.x, p.y));
//                CCNode * tile = m_pTile->getChildByTag(MAKEWORD(((int)p.x), ((int)p.y)));
//                
//                if (oim->GetType() == OBJECT_TYPE_FIELD && ((Field *)oim)->GetCrop() != NULL)
//                    tile->removeChildByTag(TILE_CROP, true);
//                tile->removeChildByTag(TILE_BUILDING, true); //TILE_BUILDING == TILE_FARM
//                
//                m_pTalkbox->setVisible(false);
                //m_pSystem->SellObject(oim);
                
                ThreadObject complete(this);
                complete.pFunc = THREAD_FUNC(Map::_removeObjectSprite);
                complete.parameter = oim;
                
                ThreadObject fail(this);
                fail.pFunc = THREAD_FUNC(Map::_removeObjectSprite);
                fail.parameter = oim;
                
                m_pSystem->SellObject(oim, this, complete, fail, true);
            }
            else
            {
                if (m_pTalkbox->isVisible())
                    m_pTalkbox->setVisible(false);
                
                CCPoint p = pTouch->locationInView();
                p = CCDirector::sharedDirector()->convertToGL(p);
                
                int t = _cursorXY(p);
                int x = LOWORD(t), y = HIWORD(t);
                
                int prev = _cursorXY(m_touch[0]);
                
                if (prev == t)
                    if (!(x < -m_width / 2 || x > m_width / 2 || y < -m_width / 2 || y > m_width / 2))
                    {
                        POINT<int> pos(x, y);
                        
                        ObjectInMap *pObj = m_pSystem->FindObject(pos);
                        
                        if (pObj)
                        {
                            StartProcess(x, y);
                            
                            ThreadObject fail(this), complete(this);
                            
                            complete.pFunc      = THREAD_FUNC(Map::_SyncPos);
                            
                            fail.pFunc          = THREAD_FUNC(Map::_ShowTalkBox);
                            fail.parameter      = new TALKBOX(NULL, x, y);
                            
                            m_pSystem->Harvest(pos, &pObj, complete, fail);
                        }
                    }
            }
        }
    }
    
    
    CCSetIterator i = pTouches->begin();
    
    while (i != pTouches->end() && m_touchCnt >= 0)
    {
        if (m_touchCnt >= 1 && m_touchID[1] == static_cast<CCTouch *>(*i)->getID())
        {
            m_touch[0] = m_touch[1];
            m_touchID[0] = m_touchID[1];
        }
        
        --m_touchCnt;
        ++i;
    }
    
    m_isDragging = false;
    m_isScaling = false;
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