//
//  MapTile.cpp
//  MobileSNG
//
//  Created by 강 건우 on 12. 7. 25..
//  Copyright (c) 2012년 code.athei@gmail.com. All rights reserved.
//

#include "MapTile.h"

using namespace cocos2d;

int MapTile::width = 50;
int MapTile::height = 30;

MapTile::MapTile() : m_pSpr(NULL)
{
    
}

MapTile::~MapTile()
{
    removeAllChildrenWithCleanup(true);
    
    SAFE_DELETE(m_pSpr)
}

bool MapTile::init()
{
    if (!CCLayer::init())
        return false;
    
    m_pSpr = new CCSprite();
    m_pSpr->initWithFile("Tile.png");
    m_pSpr->setAnchorPoint(ccp(0.5, 0.5));
    m_pSpr->setPosition(ccp(0, 0));
    addChild(m_pSpr, 0);
    
    return true;
}