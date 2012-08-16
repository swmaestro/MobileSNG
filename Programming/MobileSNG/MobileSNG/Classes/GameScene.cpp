#include "GameScene.h"
#include "Map.h"
#include "Shop.h"
#include "GameSystem.h"
#include "MapMgr.h"

#include "Join.h"

using namespace cocos2d;

GameScene::GameScene() : m_pSystem(NULL), m_pMap(NULL), m_pShop(NULL), m_pCurrentUI(NULL), m_pUIMgr(NULL)
{
    
}

GameScene::~GameScene()
{
    removeAllChildrenWithCleanup(true);
    
    delete m_pSystem;
    delete m_pMap;
    delete m_pShop;
    delete m_pUIMgr;
    
    m_pCurrentUI = NULL;
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

    m_pSystem = new GameSystem();
    if (!m_pSystem->initialize("ObjectDB.sqlite"))
        return false;
    
    if (!_initUIMgr())
        return false;
    
    if (!_initShop())
        return false;
    
    CCSize wsize = CCDirector::sharedDirector()->getWinSize();
    
    m_pMap = new Map();
    m_pMap->init(m_pSystem);
    m_pMap->setAnchorPoint(ccp(0.5, 0.5));
    m_pMap->setScale(1);
    m_pMap->setPosition(ccp(wsize.width / 2, wsize.height / 2));
    m_pMap->setVisible(false);
    addChild(m_pMap, 0);
    
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

bool GameScene::_initShop()
{
    m_pShop = new Shop();
    m_pShop->init(this);
    m_pShop->setVisible(false);
    
    ObjectInfoMgr * infoMgr = m_pSystem->GetInfoMgr();
    
    std::vector<CropInfo*> infoCrop = infoMgr->GetAllCropInfo();
    std::vector<BuildingInfo*> infoBuilding = infoMgr->GetAllBuildingInfo();
    
//    char fileName[32];
    
    for (int i = 0; i < infoCrop.size(); ++i)
    {
        std::string fileName = infoCrop[i]->GetName() + "/"
                             + infoCrop[i]->GetName() + ".png";
//        
//        sprintf(fileName, "%s/%s.png", infoCrop[i].name, infoCrop[i].name);
                
        m_pShop->addItem(OBJ_CROP, infoCrop[i]->GetName().data(), fileName.data(),
                         infoCrop[i]->GetPrice(), 0, infoCrop[i]->GetObjInfo().GetTime(), 0, infoCrop[i]->GetObjInfo().GetReward());
    }
    
    for (int i = 0; i < infoBuilding.size(); ++i)
    {
        std::string fileName = infoBuilding[i]->GetName() + "/" + infoBuilding[i]->GetName() + ".png";
//        sprintf(fileName, "%s/%s.png", infoBuilding[i].name, infoBuilding[i].name);
        
        m_pShop->addItem(OBJ_BUILDING, infoBuilding[i]->GetName().data(), fileName.data(),
                         infoBuilding[i]->GetPrice(), 0, infoBuilding[i]->GetObjInfo().GetTime(), 0, infoBuilding[i]->GetObjInfo().GetReward());
    }
    
    addChild(m_pShop, 0);
    
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
    m_pMap->beginEdit(type, id);
    m_pUIMgr->ChangeUI(UI_EDIT);
    _changeUI(m_pMap);
}