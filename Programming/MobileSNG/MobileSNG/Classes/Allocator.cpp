//
//  Allocator.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 10..
//
//

#include "Allocator.h"
#include "MapMgr.h"
#include "SceneGame.h"
#include "Shop.h"

#include "Map.h"
#include "Building.h"
#include "Field.h"

using namespace cocos2d;

Allocator::~Allocator()
{
    m_vec.clear();
    removeAllChildrenWithCleanup(true);
}

void Allocator::init(MapMgr * mapMgr, int width, int type, int id)
{
    m_pMapMgr = mapMgr;
    m_width = width;
    m_touch = NULL;
    
    m_vec.clear();
    removeAllChildrenWithCleanup(true);
    
    for (int i = 0; i < m_width; ++i)
        for (int j = 0; j < m_width; ++j)
        {
            CCSprite * spr = CCSprite::create("EditTile.png");
            spr->setAnchorPoint(ccp(0.5, 0.5));
            spr->setPosition(ccp((i + j - m_width + 1) * Map::tileWidth / 2, (j - i) * Map::tileHeight / 2));
            addChild(spr, i - j + m_width);
        }
    
    m_type = type;
    m_id = id;
}

void Allocator::Apply()
{
    for (int i = 0; i < m_vec.size(); ++i)
    {
        ObjectInMap oim;
        
        switch (m_type)
        {
            case OBJ_CROP:
                dynamic_cast<Field *>(m_pMapMgr->FindObject(POINT<int>(m_vec[i] / m_width, m_vec[i] % m_width)))->addCrop(m_id);
                break;
                
            case OBJ_BUILDING:
                oim.m_id = m_id;
                oim.m_direction = OBJECT_DIRECTION_LEFT;
                oim.m_position = POINT<int>(m_vec[i] / m_width, m_vec[i] % m_width);
                oim.m_size = SIZE<int>(1, 1);
                oim.m_state = 0;
                
                {
                    Building b(&oim, 0);
                    m_pMapMgr->addObject(b, 0);
                }
                
                break;
                
            case OBJ_FARM:
                oim.m_id = m_id;
                oim.m_direction = OBJECT_DIRECTION_LEFT;
                oim.m_position = POINT<int>(m_vec[i] / m_width, m_vec[i] % m_width);
                oim.m_size = SIZE<int>(1, 1);
                oim.m_state = 0;
            
                {
                    Field f(&oim);
                    m_pMapMgr->addObject(f, 0);
                }
                break;
        }
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
        if (m_vec[t] == i * m_width + j)
            return;
    
    char temp[30];
    if (m_type == OBJ_FARM)
        sprintf(temp, "Farm.png");
    else
        sprintf(temp, "%s/04.png", tempString[m_type][m_id]);
    
    m_touch = CCSprite::create(temp);
    
    if (m_type == OBJ_FARM)
        m_touch->setAnchorPoint(ccp(0.5, 0.5));
    else
        m_touch->setAnchorPoint(ccp(0.5, 0.3));
    
    m_touch->setPosition(ccp((i + j - m_width + 1) * Map::tileWidth / 2, (j - i) * Map::tileHeight / 2));
    m_touch->setOpacity(180);
    addChild(m_touch, 0);
}

void Allocator::TouchesMove()
{
    if (m_touch)
    {
        removeChild(m_touch, true);
        m_touch = NULL;
    }
}

void Allocator::TouchesEnd(int i, int j)
{
    if (m_touch)
    {
        m_vec.push_back(i * m_width + j);
        m_touch = NULL;
    }
}