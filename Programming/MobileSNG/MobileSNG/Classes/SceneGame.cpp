#include "SceneGame.h"
#include "Map.h"
#include "Shop.h"
#include "GameSystem.h"
#include "MapMgr.h"

using namespace cocos2d;

char * tempString[2][5] =
{
  "CandyCane",    "MushMallow", "JellyBean", " ", " ",
  "HauntedHouse", " ",          " ",         " ", " ",
};

SceneGame::SceneGame() : m_pSystem(NULL), m_pMap(NULL), m_pShop(NULL), m_pCurrentUI(NULL), m_pUIMgr(NULL)
{
    
}

SceneGame::~SceneGame()
{
    removeAllChildrenWithCleanup(true);
    
    SAFE_DELETE(m_pSystem);
    SAFE_DELETE(m_pMap);
    SAFE_DELETE(m_pShop);
    SAFE_DELETE(m_pUIMgr);
    
    m_pCurrentUI = NULL;
}

bool SceneGame::init()
{
    if (!CCLayer::init())     
        return false;
    
    /*m_pUIMgr = new UIMgr;
    m_pUIMgr->CreateUI(reinterpret_cast<CCScene*>(this),
                       menu_selector(SceneGame::_shopFunc),
                       menu_selector(SceneGame::_editFunc), 
                       menu_selector(SceneGame::_flatFunc),
                       menu_selector(SceneGame::_shopCloseFunc));
    addChild(m_pUIMgr, 1);
     
     m_pUIMgr->MapUI();
     _changeUI(m_pMap);
    */
    
    if (!_initUIMgr())
        return false;
    
    m_pSystem = new GameSystem();
    if (!m_pSystem->initialize(""))
        return false;
    
    CCSize wsize = CCDirector::sharedDirector()->getWinSize();
    
    m_pMap = new Map();
    m_pMap->init();
    m_pMap->setAnchorPoint(ccp(0.5, 0.5));
    m_pMap->setScale(1);
    m_pMap->setPosition(ccp(wsize.width / 2, wsize.height / 2));
    m_pMap->setVisible(false);
    addChild(m_pMap, 0);
    
    m_pShop = new Shop();
    m_pShop->init(this);
    m_pShop->setVisible(false);
    addChild(m_pShop, 0);
    
    setTouchEnabled(true);
    
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMap);

    return true;
}

bool SceneGame::_initUIMgr()
{
    m_pUIMgr = new UIMgr;
    
    m_pUIMgr->AppendUI(UI_MAP, "Edit.png", "Edit.png", 
                       ccp(145, -130), this, menu_selector(SceneGame::_editFunc));
    m_pUIMgr->AppendUI(UI_MAP, "Friends.png", "Friends.png", 
                       ccp(-200, -120), this, menu_selector(SceneGame::_friendsFunc));
    m_pUIMgr->AppendUI(UI_MAP, "Shop.png", "Shop.png", 
                       ccp(200, -120), this, menu_selector(SceneGame::_shopFunc));
    
    m_pUIMgr->AppendUI(UI_SHOP, "Shop-Close.png", "Shop-Close.png",
                       ccp(210, 130), this, menu_selector(SceneGame::_shopCloseFunc));
    
    addChild(m_pUIMgr, 1);
    
    return true;
}

void SceneGame::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent)
{
    if (!m_pCurrentUI && !(m_pCurrentUI = m_pMap))
        return;
    
    m_pCurrentUI->ccTouchesBegan(pTouches, pEvent);
}

void SceneGame::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (!m_pCurrentUI && !(m_pCurrentUI = m_pMap))
        return;
    
    m_pCurrentUI->ccTouchesMoved(pTouches, pEvent);
}

void SceneGame::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if (!m_pCurrentUI && !(m_pCurrentUI = m_pMap))
        return;
    
    m_pCurrentUI->ccTouchesEnded(pTouches, pEvent);
}

void SceneGame::_shopFunc(CCObject *pSender)
{
    m_pUIMgr->ChangeUI(UI_SHOP);
    _changeUI(m_pShop);
    
    CCLog(__FUNCTION__);
}

void SceneGame::_shopCloseFunc(CCObject *pSender)
{
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMap);
    
    CCLog(__FUNCTION__);
}

void SceneGame::_friendsFunc(CCObject *pSender)
{
    CCLog(__FUNCTION__);
}

void SceneGame::_editFunc(CCObject *pSender)
{
    CCLog(__FUNCTION__);
}

void SceneGame::_flatFunc(CCObject *pSender)
{
    CCLog(__FUNCTION__);
}

void SceneGame::_changeUI(cocos2d::CCLayer * ui)
{
    if (m_pCurrentUI)
        m_pCurrentUI->setVisible(false);
    if (ui)
        ui->setVisible(true);
    
    m_pCurrentUI = ui;
}

void SceneGame::alloc(int type, int id)
{
//    m_pSystem->GetMapMgr()->addObject();
    m_pMap->beginEdit(type, id);
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMap);
}