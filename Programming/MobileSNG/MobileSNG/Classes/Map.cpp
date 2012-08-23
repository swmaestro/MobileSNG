//
//  Map.cpp
//  MobileSNG
//
//  Created by 강 건우 on 12. 7. 24..
//  Copyright (c) 2012년 code.athei@gmail.com. All rights reserved.
//

#include "Map.h"
#include "Allocator.h"
#include "Talkbox.h"

#include "Shop.h"
#include "GameScene.h"
#include "GameSystem.h"

using namespace cocos2d;

int Map::width = 480 * 4;
int Map::height = 320 * 4;
int Map::tileWidth = 100;
int Map::tileHeight = 60;

Map::Map(int & width) : m_pTile(NULL), m_pAllocator(NULL), m_pTalkbox(NULL),
                m_width(width), m_touchCnt(-1),
                m_isDragging(false), m_isScaling(false),
                m_isAllocating(false), m_isEditing(false)
{
    
}

Map::~Map()
{
    removeAllChildrenWithCleanup(true);
    
    delete m_pAllocator;
}


bool Map::init(GameSystem * system)
{
    if (!CCLayer::init())
        return false;
    
    m_pSystem = system;
    
    _initTile();
    
    CCSprite * bg = CCSprite::create("Background.png");
    bg->setAnchorPoint(ccp(0.5, 0.5));
    bg->setPosition(ccp(0, 0));
    bg->setScale(4);
    addChild(bg, 0);
    
    m_pAllocator = new Allocator(m_pTile, m_width);
    
    m_pTalkbox = Talkbox::create();
    m_pTalkbox->setAnchorPoint(ccp(0, 0));
    m_pTalkbox->setVisible(false);
    addChild(m_pTalkbox, 2);
    
    scheduleUpdate();
    
    return true;
}

void Map::update(float dt)
{
    MapMgr * mapMgr = m_pSystem->GetMapMgr();
    std::vector<ObjectInMap *> object = mapMgr->GetAllObject();
    std::vector<ObjectInMap *>::iterator i;
    
    for (i = object.begin(); i != object.end(); ++i)
        if ((*i)->UpdateSystem())
            SyncPos(*i);
}

void Map::SyncPos(ObjectInMap *oim)
{
    ObjectInfoMgr * infoMgr = m_pSystem->GetInfoMgr();

    CCNode * tile = m_pTile->getChildByTag(MAKEWORD(oim->m_position.x, oim->m_position.y));
    std::string filename;
    
    if (oim->GetType() == OBJECT_TYPE_BUILDING)
    {
        Building * b = dynamic_cast<Building *>(oim);
        BuildingInfo * info;
        CCSprite * spr = dynamic_cast<CCSprite *>(tile->getChildByTag(TILE_BUILDING));
        
        infoMgr->searchInfo(b->GetID(), &info);
        filename = info->GetName();
        
        switch (b->m_state)
        {
            case BUILDING_STATE_UNDER_CONSTRUCTION_1:
                filename += "/01.png";
                break;
                
            case BUILDING_STATE_UNDER_CONSTRUCTION_2:
                filename += "/02.png";
                break;
                
            case BUILDING_STATE_WORKING:
                filename += "/03.png";
                break;
                
            case BUILDING_STATE_DONE:
                filename += "/Complete.png";
                break;
        }
        
        CCTexture2D * tex = CCTextureCache::sharedTextureCache()->addImage(filename.c_str());
        spr->setTexture(tex);
    }
    else if (oim->GetType() == OBJECT_TYPE_FIELD)
    {
        Field * f = dynamic_cast<Field *>(oim);
        Crop * c = f->GetCrop();
        
        if (!c)
        {
            tile->removeChildByTag(TILE_CROP, true);
            return;
        }
        
        CropInfo * info;
        CCSprite * spr = dynamic_cast<CCSprite *>(tile->getChildByTag(TILE_CROP));
        
        infoMgr->searchInfo(c->GetID(), &info);
        filename = info->GetName();
        
        switch (c->GetState())
        {
            case CROP_STATE_GROW_1:
                filename += "/01.png";
                break;
                
            case CROP_STATE_GROW_2:
                filename += "/02.png";
                break;
                
            case CROP_STATE_GROW_3:
                filename += "/03.png";
                break;
                
            case CROP_STATE_DONE:
                filename += "/Complete.png";
                break;
        }
        
        CCTexture2D * tex = CCTextureCache::sharedTextureCache()->addImage(filename.c_str());
        spr->setTexture(tex);
    }
}

void Map::_initTile()
{
    if (m_pTile)
        removeChild(m_pTile, true);
 
    m_pTile = CCLayer::create();
    
    for (int i = -m_width / 2; i <= m_width / 2; ++i)
        for (int j = -m_width / 2; j <= m_width / 2; ++j)
        {
            CCLayer * tile = CCLayer::create();
            tile->setAnchorPoint(ccp(0.5, 0.5));
            tile->setPosition(ccp((i - j) * tileWidth / 2, (i + j) * tileHeight / 2));
            
            CCSprite * spr = CCSprite::create("Tile.png");
            spr->setAnchorPoint(ccp(0.5, 0.5));
            
            tile->addChild(spr, TILE_NONE, TILE_NONE);
            m_pTile->addChild(tile, m_width - i - j, MAKEWORD(i, j));
        }
    
    addChild(m_pTile, 1);
}

int Map::_cursorXY(CCPoint cur)
{
    float scale = getScale();
    
    CCPoint o = ccpSub(getPosition(), ccp((scale - 1) * 240, (scale - 1) * 160));
    
    cur = ccpSub(cur, o);
    cur = ccpMult(cur, 1 / scale);
    
    cur.x /= tileWidth / 2;
    cur.y /= tileHeight / 2;
    CCPoint t = ccp((cur.x + cur.y) / 2, (cur.y - cur.x) / 2);
    
    int x = round(t.x);
    int y = round(t.y);
    
    int result = MAKEWORD(x, y);
    return result;
}

void Map::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent)
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

void Map::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
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

void Map::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
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
    
    if (!m_isDragging && !m_isScaling)
    {
        if (m_isAllocating)
            m_pAllocator->TouchesEnd();
        else
        {
            if (m_pTalkbox->isVisible())
                m_pTalkbox->setVisible(false);
            
            CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
            
            CCPoint p = pTouch->locationInView();
            p = CCDirector::sharedDirector()->convertToGL(p);
            
            int t = _cursorXY(p);
            int x = LOWORD(t), y = HIWORD(t);
            
            if (!(x < -m_width / 2 || x > m_width / 2 || y < -m_width / 2 || y > m_width / 2))
            {
                POINT<int> pos(x, y);

//              if (m_pSystem->Harvest(pos, NULL))
//                  SyncPos(m_pSystem->GetMapMgr()->FindObject(pos));
                
                ObjectInMap *pObj;
                
                if(m_pSystem->Harvest(pos, &pObj))
                {
                    SyncPos(pObj);
                }
                else
                {
                    m_pTalkbox->setPosition(ccp((x - y) * tileWidth / 2, (x + y) * tileHeight / 2));
                    m_pTalkbox->setVisible(true);
                }
            }
        }
    }
    
    m_isDragging = false;
    m_isScaling = false;
}

float Map::filtScale(float scale)
{
    if (scale < 0.5) scale = 0.5;
    if (scale > 1.5) scale = 1.5;
    
    setScale(scale);
    m_pTalkbox->setScale(1 / scale);
    
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
    
    setPosition(pos);
    
    return pos;
}

void Map::beginEdit()
{
    
}

void Map::beginEdit(int type, int id)
{
    m_isAllocating = true;
    
    m_pAllocator->init(m_pSystem, type, id);
}

void Map::endEdit(bool apply)
{
    if (m_isAllocating)
    {
        if (apply)
            m_pAllocator->Apply();
        
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