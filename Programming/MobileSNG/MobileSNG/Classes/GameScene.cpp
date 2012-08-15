#include "GameScene.h"
#include "Map.h"
#include "Shop.h"
#include "GameSystem.h"
#include "MapMgr.h"

#include "Join.h"

using namespace cocos2d;

const char * tempString[3][5] =
{
  "CandyCane",    "MushMallow", "JellyBean", NULL, NULL,
  "HauntedHouse", NULL,         NULL,        NULL, NULL,
  "Farm"
};

GameScene::GameScene() : m_pSystem(NULL), m_pMap(NULL), m_pShop(NULL), m_pCurrentUI(NULL), m_pUIMgr(NULL)
{
    
}

GameScene::~GameScene()
{
    removeAllChildrenWithCleanup(true);
    
    SAFE_DELETE(m_pSystem);
    SAFE_DELETE(m_pMap);
    SAFE_DELETE(m_pShop);
    SAFE_DELETE(m_pUIMgr);
    
    m_pCurrentUI = NULL;
}

bool GameScene::init()
{
    if (!CCLayer::init())     
        return false;
    
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

bool GameScene::_initUIMgr()
{
    m_pUIMgr = new UIMgr(this);
    
    //////////////////UI_MAP
    
    m_pUIMgr->AppendUI(UI_MAP, ccp(145, -130), "Edit.png", "Edit.png", UI_FUNC(_editFunc));
    m_pUIMgr->AppendUI(UI_MAP, ccp(-200, -120), "Friends.png", "Friends.png", UI_FUNC(_friendsFunc));
    m_pUIMgr->AppendUI(UI_MAP, ccp(200, -120), "Shop.png", "Shop.png", UI_FUNC(_shopFunc));
    
    //////////////////UI_EDIT
    
    m_pUIMgr->AppendUI(UI_EDIT, ccp(200, -120), "Icon-72.png", "Icon-72.png", UI_FUNC(_editApplyFunc));
    m_pUIMgr->AppendUI(UI_EDIT, ccp(145, -130), "Icon-Small-50.png", "Icon-Small-50.png", UI_FUNC(_editCancelFunc));

    //////////////////UI_SHOP
    
    m_pUIMgr->AppendUI(UI_SHOP, ccp(210, 130), "Shop-Close.png", "Shop-Close.png", UI_FUNC(_shopCloseFunc));
    
    /////////////////////////
    
    addChild(m_pUIMgr, 1);
    
    return true;
}

void GameScene::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent)
{
    if (!m_pCurrentUI && !(m_pCurrentUI = m_pMap))
        return;
    
    m_pCurrentUI->ccTouchesBegan(pTouches, pEvent);
}

void GameScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (!m_pCurrentUI && !(m_pCurrentUI = m_pMap))
        return;
    
    m_pCurrentUI->ccTouchesMoved(pTouches, pEvent);
}

void GameScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if (!m_pCurrentUI && !(m_pCurrentUI = m_pMap))
        return;
    
    m_pCurrentUI->ccTouchesEnded(pTouches, pEvent);
}

void GameScene::_shopFunc(CCObject *pSender)
{
    m_pUIMgr->ChangeUI(UI_SHOP);
    _changeUI(m_pShop);
    
    CCLog(__FUNCTION__);
}

void GameScene::_friendsFunc(CCObject *pSender)
{
    CCLog(__FUNCTION__);
}

void GameScene::_editFunc(CCObject *pSender)
{
    CCLog(__FUNCTION__);
}

void GameScene::_flatFunc(CCObject *pSender)
{
    CCLog(__FUNCTION__);
}

void GameScene::_editApplyFunc(CCObject *pSender)
{
    m_pMap->endEdit(m_pSystem->GetMapMgr());
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMap);
}

void GameScene::_editCancelFunc(CCObject *pSender)
{
    m_pMap->endEdit(false);
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMap);
}

void GameScene::_shopCloseFunc(CCObject *pSender)
{
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMap);
}

void GameScene::_changeUI(cocos2d::CCLayer * ui)
{
    if (m_pCurrentUI)
        m_pCurrentUI->setVisible(false);
    if (ui)
        ui->setVisible(true);
    
    m_pCurrentUI = ui;
}

void GameScene::alloc(int type, int id)
{
    m_pMap->beginEdit(m_pSystem->GetMapMgr(), type, id);
    m_pUIMgr->ChangeUI(UI_EDIT);
    _changeUI(m_pMap);
}