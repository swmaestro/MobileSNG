//
//  Allocator.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 10..
//
//

#include "Allocator.h"
#include "MapMgr.h"
#include "ObjectInfoMgr.h"

#include "GameScene.h"
#include "Shop.h"
#include "Map.h"

#include "Building.h"
#include "Field.h"

using namespace cocos2d;

Allocator::Allocator(CCLayer *& tile) : m_tile(tile)
{
    
}

void Allocator::init(MapMgr * mapMgr, ObjectInfoMgr * infoMgr, int width, int type, int id)
{
    m_pMapMgr = mapMgr;
    m_pInfoMgr = infoMgr;
    
    m_width = width;
    m_touch = NULL;
    
    m_vec.clear();
    
    for (int i = -m_width / 2; i <= m_width / 2; ++i)
        for (int j = -m_width / 2; j <= m_width / 2; ++j)
        {
            CCNode * tile = m_tile->getChildByTag(MAKEWORD(i, j));
            
            CCSprite * spr = CCSprite::create("EditTile.png");
            spr->setAnchorPoint(ccp(0.5, 0.5));
            tile->addChild(spr, TILE_EDIT, TILE_EDIT);
        }
    
    m_type = type;
    m_id = id;
    
    switch (m_type)
    {
        case OBJ_FARM:
        case OBJ_BUILDING:
        {
            BUILDING_INFO info;
            infoMgr->searchInfo(m_id, &info);
            m_name = info.name;
            break;
        }
            
        case OBJ_CROP:
        {
            CROP_INFO info;
            infoMgr->searchInfo(m_id, &info);
            m_name = info.name;
            break;
        }
    }
}

void Allocator::Apply()
{
    for (int i = 0; i < m_vec.size(); ++i)
    {
        CCNode * tile = m_tile->getChildByTag(m_vec[i]);
        
        if (tile == NULL)
            continue;
        
        std::string filename = m_name + "/01.png";
        
        CCSprite * spr = CCSprite::create(filename.c_str());
        spr->setAnchorPoint(ccp(0.5, 0.3));
        
        ObjectInMap oim;
        
        switch (m_type)
        {
            case OBJ_CROP:
                {
                    Field * f = dynamic_cast<Field *>(m_pMapMgr->FindObject(POINT<int>(LOWORD(m_vec[i]), HIWORD(m_vec[i]))));
                    f->addCrop(m_id);
                }
                
                tile->addChild(spr, TILE_CROP, TILE_CROP);
                break;
                
            case OBJ_BUILDING:
                oim.m_id = m_id;
                oim.m_direction = OBJECT_DIRECTION_LEFT;
                oim.m_position = POINT<int>(LOWORD(m_vec[i]), HIWORD(m_vec[i]));
                oim.m_size = SIZE<int>(1, 1);
                oim.m_state = 0;
                
                {
                    Building b(&oim, 0);
                    m_pMapMgr->addObject(b, 0);
                }
                
                tile->addChild(spr, TILE_BUILDING, TILE_BUILDING);
                break;
                
            case OBJ_FARM:
                oim.m_id = m_id;
                oim.m_direction = OBJECT_DIRECTION_LEFT;
                oim.m_position = POINT<int>(LOWORD(m_vec[i]), HIWORD(m_vec[i]));
                oim.m_size = SIZE<int>(1, 1);
                oim.m_state = 0;
            
                {
                    Field f(&oim);
                    m_pMapMgr->addObject(f, 0);
                }
                
                tile->addChild(spr, TILE_FARM, TILE_FARM);
                break;
        }
    }
}

void Allocator::Clear()
{
    for (int i = -m_width / 2; i <= m_width / 2; ++i)
        for (int j = -m_width / 2; j <= m_width / 2; ++j)
        {
            CCNode * tile = m_tile->getChildByTag(MAKEWORD(i, j));
            tile->removeChildByTag(TILE_EDIT, true);
        }
            
    
    for (int i = 0; i < m_vec.size(); ++i)
    {
        CCNode * tile = m_tile->getChildByTag(m_vec[i]);
        tile->removeChildByTag(TILE_PREVIEW, true);
    }
}

void Allocator::TouchesBegin(int i, int j)
{
    if (m_type == OBJ_BUILDING)
    {
        if (m_pMapMgr->isObjectInMap(POINT<int>(i, j)))
            return;
    }
    else if (m_type == OBJ_CROP)
    {
        if (!m_pMapMgr->isObjectInMap(POINT<int>(i, j)))
            return;
        
        ObjectInMap * obj = m_pMapMgr->FindObject(POINT<int>(i, j));
       
        if (obj->m_type != OBJECT_TYPE_FIELD)
            return;
        
        if (static_cast<Field *>(obj)->hasCrop())
            return;
    }
    
    for (int t = 0; t < m_vec.size(); ++t)
        if (m_vec[t] == MAKEWORD(i, j))
            return;
    
    CCNode * tile = m_tile->getChildByTag(MAKEWORD(i, j));
    if (!tile)
        return;
    
    if (m_touch)
    {
        m_touch->removeFromParentAndCleanup(true);
        m_touch = NULL;
    }
    
    std::string filename = m_name + "/Complete.png";
    
    m_touch = CCSprite::create(filename.c_str());
    m_touch->setOpacity(180);
    m_touch->setAnchorPoint(ccp(0.5, 0.3));
    
    tile->addChild(m_touch, TILE_PREVIEW, TILE_PREVIEW);
}

void Allocator::TouchesMove()
{
    if (m_touch)
    {
        m_touch->removeFromParentAndCleanup(true);
        m_touch = NULL;
    }
}

void Allocator::TouchesEnd()
{
    if (m_touch)
    {
        m_vec.push_back(m_touch->getParent()->getTag());
        m_touch = NULL;
    }
}