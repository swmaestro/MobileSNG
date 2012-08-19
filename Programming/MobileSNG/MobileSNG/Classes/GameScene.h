#pragma once

#include "cocos2d.h"
#include "UIMgr.h"

#define HIWORD(l)   ((short) (((long) ((l) >> 16)) & 0x0000FFFF))
#define LOWORD(l)   ((short) ((l) & 0x0000FFFF))
#define MAKEWORD(x,y) ((long)((LOWORD(x) & 0x0000FFFF)|((((long)LOWORD(y)) << 16) & 0xFFFF0000)))

class Shop;
class Map;
class GameSystem;

enum { UILAYER_TOUCH_RECIEVER, UILAYER_LABEL };

class GameScene : public cocos2d::CCLayer
{
private:
    GameSystem          * m_pSystem;
    
    Map                 * m_pMap;
    Shop                * m_pShop;
    
    cocos2d::CCLayer    * m_pMapUI, * m_pShopUI;
    cocos2d::CCLayer    * m_pCurrentUI;
    
    UIMgr               * m_pUIMgr;
    
    int                   m_width;
    
public:
    GameScene();
    ~GameScene();
    
public:
    bool init();
    
    static cocos2d::CCScene * scene();
    LAYER_CREATE_FUNC(GameScene);
    
    virtual void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);

private:
    bool _initUIMgr();
    bool _initShop();
    bool _initLabel();
    
    void _changeUI(cocos2d::CCLayer * ui);
    
    void _shopFunc(CCObject *pSender);
    void _friendsFunc(CCObject *pSender);
    void _editFunc(CCObject *pSender);
    void _flatFunc(CCObject *pSender);
    
    void _editApplyFunc(CCObject *pSender);
    void _editCancelFunc(CCObject *pSender);
    
    void _shopCloseFunc(CCObject *pSender);
    
    void update(float dt);
    
public:
    void alloc(int type, int id);
};