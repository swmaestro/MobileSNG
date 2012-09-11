//
//  Map.cpp
//  MobileSNG
//
//  Created by 강 건우 on 12. 7. 24..
//  Copyright (c) 2012년 code.athei@gmail.com. All rights reserved.
//

#include "Map.h"
#include "Talkbox.h"

#include "Shop.h"
#include "GameScene.h"
#include "GameSystem.h"

using namespace cocos2d;

int Map::width = 480 * 4;
int Map::height = 320 * 4;
int Map::tileWidth = 100;
int Map::tileHeight = 60;

Map::Map(int width) : m_pTile(NULL), m_pTalkbox(NULL),
                m_width(width), m_touchCnt(-1),
                m_isDragging(false), m_isScaling(false), m_pVillage(NULL)
{
    
}

Map::~Map()
{
    removeAllChildrenWithCleanup(true);
    
    SAFE_DELETE(m_pTalkbox);
    SAFE_DELETE(m_pVillage);
}

bool Map::init(GameSystem * system, FriendVillage * village)
{
    if (!CCLayer::init())
        return false;
    
    m_pSystem = system;
    m_pVillage = village;
    
    _initTile();
    
    CCSprite * bg = CCSprite::create("Background.png");
    bg->setAnchorPoint(ccp(0.5, 0.5));
    bg->setPosition(ccp(0, 0));
    bg->setScale(4);
    addChild(bg, 0);
    
//    m_pTalkbox = Talkbox::create();
//    m_pTalkbox->setAnchorPoint(ccp(0, 0));
//    m_pTalkbox->setVisible(false);
    
    m_pTalkbox = new Talkbox;
    if(m_pTalkbox->init() == false)
        return false;
    m_pTalkbox->setAnchorPoint(ccp(0, 0));
    addChild(m_pTalkbox, 2);

    scheduleUpdate();
    
    return true;
}

void Map::update(float dt)
{
    std::vector<ObjectInMap *> object = m_pVillage->GetAllObject();
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

bool Map::SyncPos(Thread *t, ObjectInMap *oim)
{
    Map *pThisClass = static_cast<Map*>(t);
    ObjectInfoMgr * infoMgr = pThisClass->m_pSystem->GetInfoMgr();

    CCNode * tile = pThisClass->m_pTile->getChildByTag(MAKEWORD(oim->m_position.x, oim->m_position.y));
    std::string filename;
    
    if (oim->GetType() == OBJECT_TYPE_BUILDING)
    {
        Building * b = dynamic_cast<Building *>(oim);
        BuildingInfo * info;
        
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
                
            case BUILDING_STATE_COMPLETE_CONSTRUCTION:
            case BUILDING_STATE_WORKING:
                filename += "/03.png";
                break;
                
            case BUILDING_STATE_DONE:
                filename += "/Complete.png";
                break;
        }
        
        tile->removeChildByTag(TILE_BUILDING, true);
        
        CCSprite * spr = CCSprite::create(filename.c_str());
        spr->setAnchorPoint(ccp(0.5, 0.3 / ((info->GetSize().width + info->GetSize().height) / 2)));
        tile->addChild(spr, TILE_BUILDING, TILE_BUILDING);
    }
    else if (oim->GetType() == OBJECT_TYPE_FIELD)
    {
        Field * f = dynamic_cast<Field *>(oim);
        Crop * c = f->GetCrop();
        
        if (!c)
        {
            tile->removeChildByTag(TILE_CROP, true);
            return true;
        }
        
        CropInfo * info;
        infoMgr->searchInfo(c->GetID(), &info);
        filename = info->GetName();
        
        switch (f->GetState())
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
                
            case CROP_STATE_FAIL:
            case CROP_STATE_DONE:
                filename += "/Complete.png";
                break;
                
            case CROP_STATE_FAIL:
                filename = "FailCrop.png";
                break;
        }
        
        tile->removeChildByTag(TILE_CROP, true);
        
        CCSprite * spr = CCSprite::create(filename.c_str());
        spr->setAnchorPoint(ccp(0.5, 0.3));
        tile->addChild(spr, TILE_CROP, TILE_CROP);
    }
    
    return true;
}

void Map::StartProcess(int i, int j)
{/*
    CCProgressTo * prg = CCProgressTo::create(1, 100);
    CCSprite * spr = CCSprite::create("Process.png");
    
    CCProgressTimer * timer = CCProgressTimer::create(spr);
    timer->setType(kCCProgressTimerTypeRadial);
    timer->setAnchorPoint(ccp(0.5, 0.5));
    timer->runAction(prg);
    
    CCNode * tile = m_pTile->getChildByTag(MAKEWORD(i, j));
    tile->removeChildByTag(TILE_PROCESS, true);
    tile->addChild(timer, TILE_PROCESS, TILE_PROCESS);
  */
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
            
            ObjectInMap * oim = m_pSystem->FindObject(POINT<int>(i, j));
            
            if (oim)
            {
                if (oim->GetType() == OBJECT_TYPE_FIELD)
                {
                    spr = CCSprite::create("Farm/Complete.png");
                    spr->setAnchorPoint(ccp(0.5, 0.3));
                    tile->addChild(spr, TILE_FARM, TILE_FARM);
                }
                
                SyncPos(this, oim);
                StartProcess(i, j);
            }
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
        CCTouch * pTouch = static_cast<CCTouch *>(*(pTouches->begin()));
            
        if (m_pTalkbox->Touch(pTouch))
        {
            CCPoint p = m_pTalkbox->GetPos();
            ObjectInMap * oim = m_pSystem->FindObject(POINT<int>(p.x, p.y));
            CCNode * tile = m_pTile->getChildByTag(MAKEWORD(((int)p.x), ((int)p.y)));
            
            if (oim->GetType() == OBJECT_TYPE_FIELD && ((Field *)oim)->GetCrop() != NULL)
                tile->removeChildByTag(TILE_CROP, true);
            tile->removeChildByTag(TILE_BUILDING, true); //TILE_BUILDING == TILE_FARM
            
            m_pSystem->SellObject(oim);
            m_pTalkbox->setVisible(false);
            return;
        }
        
        if (m_pTalkbox->isVisible())
            m_pTalkbox->setVisible(false);
        
        CCPoint p = pTouch->locationInView();
        p = CCDirector::sharedDirector()->convertToGL(p);
        
        int t = _cursorXY(p);
        int x = LOWORD(t), y = HIWORD(t);
        
        if (!(x < -m_width / 2 || x > m_width / 2 || y < -m_width / 2 || y > m_width / 2))
        {
            POINT<int> pos(x, y);
            
            ObjectInMap *pObj = NULL;
            
            ThreadObject fail(this), complete(this);
            complete.pFunc      = THREAD_FUNC(Map::SyncPos);
            fail.pFunc          = THREAD_FUNC(Map::_ShowTalkBox);
            fail.parameter      = new TALKBOX(NULL, x, y);
            
            m_pSystem->Harvest(pos, &pObj, complete, fail);                
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

//bool Map::_SyncPos(Thread *t, void *p)
//{
//    Map *pThisClass = static_cast<Map*>(t);
//    ObjectInMap *pObj = static_cast<ObjectInMap*>(p);
//    pThisClass->SyncPos(pObj);
//    return true;
//}

bool Map::_ShowTalkBox(Thread *t, void *p)
{
    Map *pThisClass = static_cast<Map*>(t);
    TALKBOX *pTalk = static_cast<TALKBOX*>(p);
    
    ObjectInMap *pObj = pTalk->pObj;
    
    int x = pTalk->x;
    int y = pTalk->y;

    delete pTalk;
    
    if((pObj && pObj->GetType() != OBJECT_TYPE_NONE) == false) return false;

    TALKBOX_TYPE type;
    
    if(pObj->GetType() == OBJECT_TYPE_BUILDING) type = TALKBOX_TYPE_BUILDING;
    else if(pObj->GetType() == OBJECT_TYPE_FIELD) type = TALKBOX_TYPE_FARM;
    else type = TALKBOX_TYPE_UNKNOWN;
    
    pThisClass->m_pTalkbox->setPosition(ccp((x - y) * tileWidth / 2, (x + y) * tileHeight / 2));
    pThisClass->m_pTalkbox->SetContent(ccp(x, y), type);
    pThisClass->m_pTalkbox->setVisible(true);
    
    printf("talk box view\n");

    return true;
}