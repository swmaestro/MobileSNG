#include "GameScene.h"
#include "PlayerMap.h"
#include "Shop.h"
#include "Friends.h"
#include "GameSystem.h"
#include "MapMgr.h"

#include "Join.h"
#include "DateInfo.h"
#include "Utility.h"

using namespace cocos2d;

GameScene::GameScene() : m_pSystem(NULL), m_pMap(NULL), m_pShop(NULL), 
            m_pMapUI(NULL), m_pShopUI(NULL), m_pCurrentUI(NULL), m_pUIMgr(NULL), m_threadHandle(NULL), m_threadID(0), m_isGameServer(true)
{
}

GameScene::~GameScene()
{
    removeAllChildrenWithCleanup(true);
    pthread_exit(0);
    
    delete m_pSystem;
    delete m_pMap;
    delete m_pShop;
    delete m_pUIMgr;
    
    m_pCurrentUI = NULL;
    
    SAFE_DELETE(m_pNetwork);
}

CCScene * GameScene::scene()
{
    CCScene * scene = CCScene::create();
    scene->addChild(GameScene::create());
    return scene;
}

bool GameScene::init()
{
    if (!CCLayer::init())     
        return false;
    
    m_pNetwork = new Network;
    m_pSystem = new GameSystem("ObjectDB.sqlite", m_pPlayerMap->getMapWidth(), m_pNetwork);
        
    m_threadID = pthread_create(&m_threadHandle, NULL, GameScene::serverUpdate, (void*)this);
    
    if( m_threadID < 0 )
        return false;
    
    if(m_pSystem->init() == false)
    {
        SAFE_DELETE(m_pSystem);
        return false;
    }
    
    m_pSystem->m_isInit = false;
    
    if (!_initUIMgr())
        return false;

    if (!_initMap())
        return false;
        
    if (!_initShop())
        return false;
    
    if (!_initFriends())
        return false;

  //  CCMessageBox(<#const char *pszMsg#>, <#const char *pszTitle#>)
    
    m_pMapUI = CCLayer::create();
    m_pMapUI->addChild(m_pPlayerMap, UILAYER_TOUCH_RECIEVER, UILAYER_TOUCH_RECIEVER);
    m_pMapUI->setVisible(false);
    addChild(m_pMapUI, 0);
    
    m_pFriendMapUI = CCLayer::create();
    m_pFriendMapUI->setVisible(false);
    addChild(m_pFriendMapUI, 0);
    
    m_pShopUI = CCLayer::create();
    m_pShopUI->addChild(m_pShop, UILAYER_TOUCH_RECIEVER, UILAYER_TOUCH_RECIEVER);
    m_pShopUI->setVisible(false);
    addChild(m_pShopUI, 0);
    
    m_pFriendsUI = CCLayer::create();
    m_pFriendsUI->addChild(m_pFriends, UILAYER_TOUCH_RECIEVER, UILAYER_TOUCH_RECIEVER);
    m_pFriendsUI->setVisible(false);
    addChild(m_pFriendsUI, 0);
    
    setTouchEnabled(true);
    
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMapUI);
    
    if (!_initLabel())
        return false;
    
    scheduleUpdate();
    
    return true;
}

void GameScene::update(float dt)
{
    CCLabelTTF * label = static_cast<CCLabelTTF *>(m_pCurrentUI->getChildByTag(UILAYER_LABEL));
    
    if (label)
    {
        Player *player = m_pSystem->GetPlayer();
        
        char temp[100];
        sprintf(temp, "Sweets : %d\nExp : %d", player->GetMoney(), player->GetExp());
        
        label->setString(temp);
    }
}

bool GameScene::_initUIMgr()
{
    m_pUIMgr = new UIMgr(this);
    
    //////////////////UI_MAP
    
    m_pUIMgr->AppendUI(UI_MAP, ccp(145, -130), "Edit.png", "Edit.png", UI_FUNC(_editFunc));
    m_pUIMgr->AppendUI(UI_MAP, ccp(-200, -120), "Friends.png", "Friends.png", UI_FUNC(_friendsFunc));
    m_pUIMgr->AppendUI(UI_MAP, ccp(200, -120), "Shop.png", "Shop.png", UI_FUNC(_shopFunc));
    
    //////////////////UI_FRIEND_MAP
    
    m_pUIMgr->AppendUI(UI_FRIEND_MAP, ccp(-200, -120), "Friends.png", "Friends.png", UI_FUNC(_friendsFunc));
    m_pUIMgr->AppendUI(UI_FRIEND_MAP, ccp(210, 130), "Shop-Close.png", "Shop-Close.png", UI_FUNC(_friendMapCloseFunc));
    
    //////////////////UI_EDIT
    
    m_pUIMgr->AppendUI(UI_EDIT, ccp(200, -120), "Button/Edit-OK.png", "Button/Edit-OK.png", UI_FUNC(_editApplyFunc));
    m_pUIMgr->AppendUI(UI_EDIT, ccp(135, -130), "Button/Edit-Cancel.png", "Button/Edit-Cancel.png", UI_FUNC(_editCancelFunc));

    //////////////////UI_SHOP
    
    m_pUIMgr->AppendUI(UI_SHOP, ccp(210, 130), "Shop-Close.png", "Shop-Close.png", UI_FUNC(_shopCloseFunc));
    
    /////////////////////////
    
    m_pUIMgr->AppendUI(UI_FRIENDS, ccp(210, 130), "Shop-Close.png", "Shop-Close.png", UI_FUNC(_friendsCloseFunc));
    
    /////////////////////////
    
    addChild(m_pUIMgr, 1);
    
    return true;
}

bool GameScene::_initMap()
{
    CCSize wsize = CCDirector::sharedDirector()->getWinSize();
    
    m_pPlayerMap = new PlayerMap(7);
    m_pPlayerMap->init(m_pSystem, m_pNetwork);
    m_pPlayerMap->setAnchorPoint(ccp(0.5, 0.5));
    m_pPlayerMap->filtScale(1);
    m_pPlayerMap->filtPosition(ccp(wsize.width / 2, wsize.height / 2));
    
    m_pMap = m_pPlayerMap;
 
    return true;
}

bool GameScene::_initShop()
{
    m_pShop = new Shop();
    m_pShop->init(this);
    
    ObjectInfoMgr * infoMgr = m_pSystem->GetInfoMgr();
    
    std::vector<CropInfo*> infoCrop = infoMgr->GetAllCropInfo();
    std::vector<BuildingInfo*> infoBuilding = infoMgr->GetAllBuildingInfo();
    
    for (int i = 0; i < infoCrop.size(); ++i)
    {
        std::string fileName = infoCrop[i]->GetName() + "/" + infoCrop[i]->GetName() + ".png";
        
        m_pShop->addItem(OBJ_CROP, infoCrop[i]->GetName().data(), fileName.data(),
                         infoCrop[i]->GetPrice(), 0, infoCrop[i]->GetObjInfo().GetTime(), 0, infoCrop[i]->GetObjInfo().GetReward());
    }
    
    for (int i = 0; i < infoBuilding.size(); ++i)
    {
        std::string fileName = infoBuilding[i]->GetName() + "/" + infoBuilding[i]->GetName() + ".png";
        
        m_pShop->addItem(OBJ_BUILDING, infoBuilding[i]->GetName().data(), fileName.data(),
                         infoBuilding[i]->GetPrice(), 0, infoBuilding[i]->GetObjInfo().GetTime(), 0, infoBuilding[i]->GetObjInfo().GetReward());
    }
    
//    CCMessageBox(<#const char *pszMsg#>, <#const char *pszTitle#>)
    
    return true;
}

bool GameScene::_initFriends()
{
    m_pFriends = new Friends(this, m_pSystem, m_pNetwork);
    m_pFriends->init();
    
    return true;
}

bool GameScene::_initLabel()
{
    CCSprite * sprite;
    CCLabelTTF * label;
    
    sprite = CCSprite::create("Status.png");
    sprite->setPosition(ccp(110, 270));
    m_pMapUI->addChild(sprite, UILAYER_LABEL);
    
    label = CCLabelTTF::create("jgojwjgp pjpa", "Ariel", 13);
    label->setAnchorPoint(ccp(0, 1));
    label->setPosition(ccp(90, 290));
    m_pMapUI->addChild(label, UILAYER_LABEL, UILAYER_LABEL);
    
    label = CCLabelTTF::create("hgwoojboo", "Ariel", 13);
    label->setAnchorPoint(ccp(0, 1));
    label->setPosition(ccp(100, 300));
//    m_pShopUI->addChild(label, UILAYER_LABEL, UILAYER_LABEL);
    
    return true;
}

void GameScene::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent)
{
    if (!m_pCurrentUI)
        return;
    
    static_cast<CCLayer *>(m_pCurrentUI->getChildByTag(UILAYER_TOUCH_RECIEVER))->ccTouchesBegan(pTouches, pEvent);
}

void GameScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (!m_pCurrentUI)
        return;
    
    static_cast<CCLayer *>(m_pCurrentUI->getChildByTag(UILAYER_TOUCH_RECIEVER))->ccTouchesMoved(pTouches, pEvent);
}

void GameScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if (!m_pCurrentUI)
        return;
    
    static_cast<CCLayer *>(m_pCurrentUI->getChildByTag(UILAYER_TOUCH_RECIEVER))->ccTouchesEnded(pTouches, pEvent);
}

void GameScene::_shopFunc(CCObject *pSender)
{
    m_pUIMgr->ChangeUI(UI_SHOP);
    _changeUI(m_pShopUI);
    
    CCLog(__FUNCTION__);
}

void GameScene::_friendsFunc(CCObject *pSender)
{
    m_pUIMgr->ChangeUI(UI_FRIENDS);
    _changeUI(m_pFriendsUI);
    
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
    m_pPlayerMap->endEdit(true);
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMapUI);
}

void GameScene::_editCancelFunc(CCObject *pSender)
{
    m_pPlayerMap->endEdit(false);
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMapUI);
}

void GameScene::_friendMapCloseFunc(CCObject *pSender)
{
    m_pFriendMapUI->removeChildByTag(UILAYER_TOUCH_RECIEVER, true);
    m_pMap = m_pPlayerMap;
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMapUI);
}

void GameScene::_shopCloseFunc(CCObject *pSender)
{
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMapUI);
}

void GameScene::_friendsCloseFunc(CCObject *pSender)
{
    m_pMap = m_pPlayerMap;
    m_pUIMgr->ChangeUI(UI_MAP);
    _changeUI(m_pMapUI);
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
    m_pPlayerMap->beginEdit(type, id);
    m_pUIMgr->ChangeUI(UI_EDIT);
    _changeUI(m_pMapUI);
}

void GameScene::Visit(Map *map)
{
    m_pFriendMapUI->removeChildByTag(UILAYER_TOUCH_RECIEVER, true);
    
    CCSize wsize = CCDirector::sharedDirector()->getWinSize();
    m_pMap = map;
    m_pMap->setAnchorPoint(ccp(0.5, 0.5));
    m_pMap->filtScale(1);
    m_pMap->filtPosition(ccp(wsize.width / 2, wsize.height / 2));
    
    m_pFriendMapUI->addChild(m_pMap, UILAYER_TOUCH_RECIEVER, UILAYER_TOUCH_RECIEVER);
    m_pUIMgr->ChangeUI(UI_FRIEND_MAP);
    _changeUI(m_pFriendMapUI);
}

void* GameScene::serverUpdate(void *p)
{
    GameScene *pScene = static_cast<GameScene*>(p);
    pScene->schedule(schedule_selector(GameScene::villageUpdate), 10);

    while (1)
    {
        if(pScene->m_isGameServer)
        {
            pScene->m_pSystem->threadUpdate();
        }
        else
        {
            
        }
    }
    
    return p;
}

void GameScene::villageUpdate(float t)
{
//    m_pSystem->VillageUpdate();
}