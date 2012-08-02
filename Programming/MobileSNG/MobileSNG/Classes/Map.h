//
//  Map.h
//  MobileSNG
//
//  Created by 강 건우 on 12. 7. 24..
//  Copyright (c) 2012년 code.athei@gmail.com. All rights reserved.
//

#ifndef MobileSNG_Map_h
#define MobileSNG_Map_h

#include "cocos2d.h"
#include "DataType.h"

class MapTile;

//CA Edit 120728 Move Touch to Game Scene

class Map : public cocos2d::CCLayer
{
private:
    cocos2d::CCSprite * m_pBackGround;
    
    cocos2d::CCPoint    m_touch[2];
    
    MapTile         *** m_arrTile;
    int                 m_width;
    
    bool                m_isDragging;
    bool                m_isScaling;
    
    static MapTile ***  _create(int width);
    static void         _release(MapTile *** tile, int width);
    
    int _cursorXY(cocos2d::CCPoint cur);
    
    void _addTile();
    void _removeTile();
    
public:
    Map();
    ~Map();
    
    virtual bool init();
    
    virtual void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    
    float filtScale(float scale);
    cocos2d::CCPoint filtPosition(cocos2d::CCPoint pos);
    
    //void expand();
};

#endif
