#pragma once

#include "cocos2d.h"
#include "UIMgr.h"

class Map;

class SceneGame : public cocos2d::CCLayer
{
private:
    Map                 * m_pMap;
    Shop                * m_pShop;
    cocos2d::CCLayer    * m_pCurrentUI;
    
    UIMgr               * m_pUIMgr;
    
public:
    SceneGame();
    ~SceneGame();
    
public:
    bool init();
    
    virtual void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    
private:
    bool _initUIMgr();
    
    void _shopFunc(CCObject *pSender);
    void _friendsFunc(CCObject *pSender);
    void _shopCloseFunc(CCObject *pSender);
    void _editFunc(CCObject *pSender);
    void _flatFunc(CCObject *pSender);
    
    void _changeUI(cocos2d::CCLayer * ui);
};