#pragma once

#include "cocos2d.h"
#include "UIMgr.h"
#include "Utility.h"
#include "Network.h"

class Shop;
class Map;
class PlayerMap;
class Friends;

class GameSystem;

enum { UILAYER_TOUCH_RECIEVER, UILAYER_LABEL };

class GameScene : public cocos2d::CCLayer
{
private:
    GameSystem          * m_pSystem;
    Network             * m_pNetwork;
    
    Map                 * m_pMap;
    PlayerMap           * m_pPlayerMap;
    Shop                * m_pShop;
    Friends             * m_pFriends;
    
    cocos2d::CCLayer    * m_pMapUI, * m_pFriendMapUI, * m_pShopUI, * m_pFriendsUI;
    cocos2d::CCLayer    * m_pCurrentUI;
    
    UIMgr               * m_pUIMgr;
    
    pthread_t             m_threadHandle;
    int                   m_threadID;
    bool                  m_isGameServer;
    
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
    bool _initMap();
    bool _initShop();
    bool _initFriends();
    bool _initLabel();
    
    void _changeUI(cocos2d::CCLayer * ui);
    
    void _shopFunc(CCObject *pSender);
    void _friendsFunc(CCObject *pSender);
    void _editFunc(CCObject *pSender);
    void _flatFunc(CCObject *pSender);
    void _statusFunc(CCObject *pSender);
    
    void _editApplyFunc(CCObject *pSender);
    void _editCancelFunc(CCObject *pSender);
    
    void _friendMapCloseFunc(CCObject *pSender);
    void _shopCloseFunc(CCObject *pSender);
    void _friendsCloseFunc(CCObject *pSender);
    
    void update(float dt);
    
    void villageUpdate(float t);
    
public:
    void alloc(int type, int id);
    
    void Visit(Map * map);
    
private:
    static void* serverUpdate(void *p);
};