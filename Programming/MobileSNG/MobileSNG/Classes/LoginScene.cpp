//
//  LoginScene.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 14..
//
//

#include "LoginScene.h"
#include "Player.h"
#include "GameScene.h"
#include "rapidxml.hpp"
#include "SystemInfo.h"
#include "Utility.h"

using namespace cocos2d;

LoginScene::LoginScene()
{
    m_pJoin     = NULL;
    m_pJoinUI       = NULL;
    
    m_pLoginUI = NULL;
    m_pLogin = NULL;
    
    m_pNet      = NULL;
    m_pBackGround = NULL;
}

LoginScene::~LoginScene()
{
    SAFE_DELETE(m_pJoinUI);
    SAFE_DELETE(m_pJoin);
    SAFE_DELETE(m_pLoginUI);
    SAFE_DELETE(m_pLogin);
    SAFE_DELETE(m_pNet);
    SAFE_DELETE(m_pBackGround);
    
    exit(0);
}

CCScene* LoginScene::scene()
{
    CCScene * scene = CCScene::create();
    scene->addChild(LoginScene::create());
    return scene;
}

bool LoginScene::init()
{
    if (!CCLayer::init())
        return false;
    
    m_pNet      = new Network;
    SystemInfo *pSystem = new SystemInfo(m_pNet);
    
    if(pSystem->isUpdatedVersion() == false)
    {
        CCDirector::sharedDirector()->end();
        delete pSystem;
        return false;
    }
    
    delete pSystem;
    
    m_pJoin     = new Join(m_pNet);
    m_pLogin    = new Login(m_pNet);

    if(Player::hasFile())
    {
        char id[32];
        char pw[32];
        
        Player::GetInfo(id, pw, NULL, NULL);
        if(m_pLogin->Logon(id, pw))
        {
            _NextScene();
            return true;
        }
    }
    
    m_pBackGround = new CCSprite;
    m_pBackGround->initWithFile("Loading.png");
    m_pBackGround->setAnchorPoint(ccp(0,0));
    m_pBackGround->setPosition(ccp(0,0));
    addChild(m_pBackGround);
        
    m_pJoinUI   = new JoinUI(this,
                             menu_selector(LoginScene::_btJoin),
                             menu_selector(LoginScene::_btChangeUI),
                             menu_selector(LoginScene::_btRepetition));
    
    m_pJoinUI->setVisible(false);
    
    m_pJoinUI->setAnchorPoint(ccp(0.5f, 0.5f));
    m_pJoinUI->setPosition(ccp(100,100));
    
    addChild(m_pJoinUI);
    
    m_pLoginUI  = new LoginUI( menu_selector(LoginScene::_btLogin),
                               menu_selector(LoginScene::_btChangeUI),
                               this);
    
    m_pLoginUI->setAnchorPoint(ccp(0.5,0.5));
    m_pLoginUI->setPosition(ccp(100,400));
    
    CCMoveTo *movAction = CCMoveTo::create(1.f, ccp(100, 100));
    m_pLoginUI->runAction(movAction);
    
    addChild(m_pLoginUI);
    
    return true;
}

#pragma mark -
#pragma mark JOIN_UI

void LoginScene::_btJoin(CCObject *pSender)
{
    CCLOG(__FUNCTION__);

    const char *id = m_pJoinUI->GetID();
    const char *pw = m_pJoinUI->GetPW();
    const char *ph = m_pJoinUI->GetPhone();
    
    if(m_pJoin->CreatAccount(id, pw, ph))
    {
        CCMessageBox("Join OK", "Join OK");
        _btChangeUI(NULL);
    }
    else
        CCMessageBox("Join Fail", "Join Fail");
    
    m_pJoinUI->AllClear();
}

void LoginScene::_btRepetition(CCObject *pSender)
{
    CCLOG(__FUNCTION__);

    const char *id = m_pJoinUI->GetID();
    
    if(m_pJoin->CheckOverlapID(id))
        CCMessageBox("아이디가 이미 존재합니다", "Error");
}

void LoginScene::_btCancel(CCObject *pSender)
{
    CCLOG(__FUNCTION__);
}

bool LoginScene::_GetUserInfo(char *userID, char **ppoutID, char **ppoutPhone, char **ppOutDate)
{
    static const char *baseURL = "http://swmaestros-sng.appspot.com/searchmember?id=%s";
    
    char url[128];
    sprintf(url, baseURL, userID);
    
    CURL_DATA data;
    if(m_pNet->connectHttp(url, &data) != CURLE_OK)
    {
        printf("%s <- Fail", __FUNCTION__);
        return false;
    }
    
    rapidxml::xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    if(xmlDoc.first_node()->first_node()->value()[0] == '0')
        return false;
    
    rapidxml::xml_node<char> *pNode = xmlDoc.first_node()->first_node()->next_sibling()->first_node();
    
    if(ppoutID)     *ppoutID = pNode->first_node()->value();
    pNode = pNode->next_sibling();
    if(ppoutPhone)  *ppoutPhone = pNode->value();
    pNode = pNode->next_sibling();
    if(ppOutDate)   *ppOutDate = pNode->value();
    
    return true;
}

#pragma mark -
#pragma mark LOGIN_UI

void LoginScene::_btLogin(CCObject *pSender)
{
    CCLOG(__FUNCTION__);
    
    const char *userID = m_pLoginUI->GetString(LOGIN_UI_ENUM_ID);
    const char *userPW = m_pLoginUI->GetString(LOGIN_UI_ENUM_PW);
    
    if(m_pLogin->Logon(userID, userPW))
    {
        char *phone;
        char *date;
        std::string phoneNum;
        
        _GetUserInfo(const_cast<char*>(userID), NULL, &phone, &date);
        
        phoneNum = phone;
        
        Player::newPlayer(userID, userPW, phone, date);
        
        printf("Login Success \n");
        CCMessageBox("Login Success!", "Login");
        _NextScene();
    }
    else
    {
        printf("Login Fail \n");
        CCMessageBox("Login Fail", "Login Fail");
    }
}

void LoginScene::_btChangeUI(CCObject *pSender)
{
    CCLOG(__FUNCTION__);
    
    CCCallFunc *func = CCCallFunc::create(this, callfunc_selector(LoginScene::_ChangeUI));
    
    //추후에 액션을 추가하든가 하시죠
//    CCSequence *seq = CCSequence::create(func);
    
    if(m_pLoginUI->isVisible())
        m_pLoginUI->runAction(func);
    else
        m_pJoinUI->runAction(func);
}

void LoginScene::_ChangeUI(CCObject *pSender)
{
    if( m_pLoginUI->isVisible() == false )
    {
        m_pLoginUI->setVisible(true);
        m_pJoinUI->setVisible(false);
    }

    else
    {
        m_pJoinUI->setVisible(true);
        m_pLoginUI->setVisible(false);
    }
}

#pragma mark -

void LoginScene::_NextScene()
{    
    CCCallFunc *p = CCCallFunc::create(this, callfunc_selector(LoginScene::_Dealloc));
    runAction(p);
}

void LoginScene::_Dealloc(CCObject *pSender)
{
    removeAllChildrenWithCleanup(true);

    SAFE_DELETE(m_pJoinUI);
    SAFE_DELETE(m_pJoin);
    SAFE_DELETE(m_pLoginUI);
    SAFE_DELETE(m_pLogin);
    SAFE_DELETE(m_pNet);
    SAFE_DELETE(m_pBackGround);
    
    CCDirector::sharedDirector()->pushScene(GameScene::scene());
}