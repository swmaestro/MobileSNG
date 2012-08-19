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

#include "Utility.h"

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
            BuildingInfo * info;
            infoMgr->searchInfo(m_id, &info);
            m_name = info->GetName();
            break;
        }
            
        case OBJ_CROP:
        {
            CropInfo * info;
            infoMgr->searchInfo(m_id, &info);
            m_name = info->GetName();
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
        
        ObjectInMap oim;
        
        switch (m_type)
        {
            case OBJ_CROP:
                {
                    Field * f = dynamic_cast<Field *>(m_pMapMgr->FindObject(POINT<int>(LOWORD(m_vec[i]), HIWORD(m_vec[i]))));
                    f->addCrop(m_id, 0, m_pInfoMgr);
                }
                
                spr->setAnchorPoint(ccp(0.5, 0.3));
                tile->addChild(spr, TILE_CROP, TILE_CROP);
                break;
                
            case OBJ_BUILDING:
                {
                    BuildingInfo * info;
                    m_pInfoMgr->searchInfo(m_id, &info);
                    spr->setAnchorPoint(ccp(0.5, 0.3 / ((info->GetSize().width + info->GetSize().height) / 2)));
                    
                    oim = ObjectInMap(0, POINT<int>(LOWORD(m_vec[i]), HIWORD(m_vec[i])),
                                      info->GetSize(), OBJECT_DIRECTION_LEFT, m_id);
                    
                    Building b(&oim, 0, m_pInfoMgr);
                    m_pMapMgr->addObject(&b, m_pInfoMgr, 0);
                }
                
                tile->addChild(spr, TILE_BUILDING, TILE_BUILDING);
                break;
                
            case OBJ_FARM:
                oim = ObjectInMap(0, POINT<int>(LOWORD(m_vec[i]), HIWORD(m_vec[i])), SIZE<int>(1, 1), OBJECT_DIRECTION_LEFT, m_id);
            
                {
                    Field f(&oim);
                    m_pMapMgr->addObject(&f, m_pInfoMgr, 0);
                }
                
                spr->setAnchorPoint(ccp(0.5, 0.3));
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
        BuildingInfo * info;
        m_pInfoMgr->searchInfo(m_id, &info);
        
        if (i > m_width / 2 - info->GetSize().width + 1 || j > m_width / 2 - info->GetSize().height + 1)
            return;
        
        if (m_pMapMgr->isObjectInMap(POINT<int>(i, j), info->GetSize()))
            return;
        
        for (int t = 0; t < m_vec.size(); ++t)
            if (intersectBoxWithBox(POINT<int>(LOWORD(m_vec[i]), HIWORD(m_vec[i])), info->GetSize(),
                                    POINT<int>(i, j), info->GetSize()))
                return;
    }
    else
    {
        if (m_type == OBJ_CROP)
        {
            if (!m_pMapMgr->isObjectInMap(POINT<int>(i, j)))
                return;
            
            ObjectInMap * obj = m_pMapMgr->FindObject(POINT<int>(i, j));
           
            if (obj->GetType() != OBJECT_TYPE_FIELD)
                return;
            
            if (static_cast<Field *>(obj)->hasCrop())
                return;
        }
       
        for (int t = 0; t < m_vec.size(); ++t)
            if (m_vec[t] == MAKEWORD(i, j))
                return;
    }
    
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
    
    if (m_type == OBJ_BUILDING)
    {
        BuildingInfo * info;
        m_pInfoMgr->searchInfo(m_id, &info);
        m_touch->setAnchorPoint(ccp(0.5, 0.3 / ((info->GetSize().width + info->GetSize().height) / 2)));
    }
    else
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