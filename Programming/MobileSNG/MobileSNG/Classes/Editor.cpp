//
//  Editor.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 10..
//
//

#include "Editor.h"
#include "MapTile.h"

#include "SceneGame.h"

using namespace cocos2d;

Editor::~Editor()
{
    removeAllChildrenWithCleanup(true);
}

void Editor::init(int width)
{
    m_width = width;
    m_isSetter = false;
    m_touch = NULL;
    
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

void Editor::init(int width, int type, int id)
{
    init(width);
    
    m_isSetter = true;
    m_setType = type;
    m_setID = id;
}

void Editor::TouchesBegin(int i, int j)
{
    if (m_isSetter)
    {
        char temp[30];
        sprintf(temp, "%s/04.png", tempString[m_setType][m_setID]);
        
        m_touch = CCSprite::create(temp);
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
        removeChild(m_touch, true);
        m_touch = NULL;
    }
}