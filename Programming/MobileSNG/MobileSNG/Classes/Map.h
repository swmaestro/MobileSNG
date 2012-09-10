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
#include "Thread.h"

class MapTile;
class MapMgr;
class Allocator;
class GameSystem;
class ObjectInMap;
class Talkbox;

//CA Edit 120728 Move Touch to Game Scene

struct TALKBOX
{
    ObjectInMap *pObj;
    int x,y;
    
    TALKBOX() : pObj(NULL), x(0), y(0) {}
    TALKBOX(ObjectInMap *pObj, int x, int y)
    { this->pObj = pObj; this->x = x; this->y = y; }
};

enum
{
    TILE_NONE, TILE_EDIT, TILE_FARM, TILE_BUILDING = TILE_FARM, TILE_CROP, TILE_PREVIEW, TILE_PROCESS,
};

class Map : public cocos2d::CCLayer, private Thread
{
protected:
    Map(int & width);
    ~Map();
    
    cocos2d::CCPoint    m_touch[2];
    int                 m_touchID[2];
    int                 m_touchCnt;
    
    cocos2d::CCLayer  * m_pTile;
    Talkbox           * m_pTalkbox;
    
    int               & m_width;
    
    bool                m_isDragging, m_isScaling;
        
    int _cursorXY(cocos2d::CCPoint cur);
    
    void _initTile();
    
    GameSystem * m_pSystem;
    
private:
//    bool _SyncPos(Thread *t, void *p);
    bool _ShowTalkBox(Thread *t,void *p);
    
public:
    static int width, height, tileWidth, tileHeight;
    
    virtual bool init(GameSystem * system);
    
    virtual void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    
    float filtScale(float scale);
    cocos2d::CCPoint filtPosition(cocos2d::CCPoint pos);
    
    bool SyncPos(Thread *t, ObjectInMap * oim);
    void StartProcess(int i, int j);
    
    void update(float dt);
    
    //void expand();
};

#endif
