//
//  Editor.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 10..
//
//

#include "Editor.h"
#include "MapTile.h"
#include "MapMgr.h"
#include "SceneGame.h"
#include "Shop.h"

using namespace cocos2d;

Editor::~Editor()
{
    m_setVec.clear();
    removeAllChildrenWithCleanup(true);
}

void Editor::init(MapMgr * mapMgr, int width)
{
    m_pMapMgr = mapMgr;
    m_width = width;
    m_isSetter = false;
    m_touch = NULL;
    
    m_setVec.clear();
    removeAllChildrenWithCleanup(true);
    
    for (int i = 0; i < m_width; ++i)
        for (int j = 0; j < m_width; ++j)
        {
            CCSprite * spr = CCSprite::create("EditTile.png");
            spr->setAnchorPoint(ccp(0.5, 0.5));
            spr->setPosition(ccp((i + j - m_width + 1) * MapTile::width / 2, (j - i) * MapTile::height / 2));
            addChild(spr, 0);
        }
}

void Editor::init(MapMgr * mapMgr, int width, int type, int id)
{
    init(mapMgr, width);
    
    m_isSetter = true;
    m_setType = type;
    m_setID = id;
}

void Editor::Apply()
{
    if (m_isSetter)
        for (int i = 0; i < m_setVec.size(); ++i)
        {
            switch (m_setType)
            {
                case OBJ_CROP:
                    break;
                    
                case OBJ_BUILDING:
                    break;
                    
                case OBJ_FARM:
                    break;
            }
        }
}

void Editor::TouchesBegin(int i, int j)
{
    if (m_isSetter)
    {
        if (m_setType == OBJ_BUILDING)
        {
            if (m_pMapMgr->isObjectInMap(POINT<int>(i, j)))
                return;
        }
        else if (m_setType == OBJ_CROP)
        {
            if (!m_pMapMgr->isObjectInMap(POINT<int>(i, j)))
                return;
            
            ObjectInMap * obj = m_pMapMgr->FindObject(POINT<int>(i, j));
           
            if (obj->m_type != OBJECT_TYPE_FIELD)
                return;
            
            if (static_cast<Field *>(obj)->hasCrop())
                return;
        }
        
        for (int t = 0; t < m_setVec.size(); ++t)
            if (m_setVec[t] == i * m_width + j)
                return;
        
        char temp[30];
        if (m_setType == OBJ_FARM)
            sprintf(temp, "Farm.png");
        else
            sprintf(temp, "%s/04.png", tempString[m_setType][m_setID]);
        
        m_touch = CCSprite::create(temp);
        
        if (m_setType == OBJ_FARM)
            m_touch->setAnchorPoint(ccp(0.5, 0.5));
        else
            m_touch->setAnchorPoint(ccp(0.5, 0.3));
        
        m_touch->setPosition(ccp((i + j - m_width + 1) * MapTile::width / 2, (j - i) * MapTile::height / 2));
        m_touch->setOpacity(180);
        addChild(m_touch, 0);
    }
}

void Editor::TouchesMove()
{
    if (m_isSetter && m_touch)
    {
        removeChild(m_touch, true);
        m_touch = NULL;
    }
}

void Editor::TouchesEnd(int i, int j)
{
    if (m_isSetter && m_touch)
    {
        m_setVec.push_back(i * m_width + j);
        m_touch = NULL;
    }
}