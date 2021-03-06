//
//  Friends.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 23..
//
//

#include "Friends.h"
#include "Map.h"
#include "rapidxml.hpp"

using namespace cocos2d;
using namespace rapidxml;

Friends::Friends(GameScene * scene, GameSystem * system, Network *pNetwork)
{
    m_pNetwork = pNetwork;
    m_pScene = scene;
    m_pSystem = system;
    m_pSocial = NULL;
    m_pTextField = NULL;
    m_pFriendVillage = NULL;
    m_pSearch        = NULL;
}

Friends::~Friends()
{
    SAFE_DELETE(m_pSocial);
    SAFE_DELETE(m_pTextField);
    SAFE_DELETE(m_pFriendVillage);
    
    m_pSearch->removeAllContent(true);
    SAFE_DELETE(m_pSearch);
}

bool Friends::init()
{
    if (!CCLayer::init())
        return false;
    
    CCSprite * pBG = CCSprite::create("Friends-Background.png");
    pBG->setAnchorPoint(ccp(0, 0));
    pBG->setPosition(ccp(0, 0));
    pBG->setOpacity(150);
    addChild(pBG, 0);
    
    m_pSocial = new Social(m_pNetwork, m_pSystem->GetPlayer());

    m_pMovable = CCLayer::create();
    addChild(m_pMovable, 1);
    
    m_pTextField = new CCTextFieldTTF;
    m_pTextField->initWithPlaceHolder("", "Ariel", 20);
    m_pTextField->setDelegate(this);
    m_pTextField->setAnchorPoint(ccp(0, 0));
    m_pTextField->setPosition(ccp(0, 300));
    m_pTextField->setColor(ccc3(255, 255, 255));
    m_pTextField->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_pTextField->setContentSize(CCSizeMake(400, 20));
    addChild(m_pTextField, 1);
    
    m_pSearch = new ListBoxSearch<UserInfo*>;
    refresh();
    
    return true;
}

void Friends::addFriendList(std::string name, int num)
{
    CCLayer * layer = CCLayer::create();
    
    CCSprite * bg = CCSprite::create("Friends-List.png");
    bg->setAnchorPoint(ccp(0, 0));
    layer->addChild(bg);
    
    CCLabelTTF * lb = CCLabelTTF::create(name.c_str(), "Ariel", 20);
    lb->setPosition(ccp(50, 30));
    lb->setHorizontalAlignment(kCCTextAlignmentLeft);
    layer->addChild(lb, 0, 0);
    
    layer->setPosition(ccp(0, 320 - 90 - num * 60));
    m_pMovable->addChild(layer, 0, num);
    
    //UserInfo *pUserInfo = _createUserInfo(m_pNetwork, name.data());
    //m_pSearch->pushContent(pUserInfo->userID.data(), pUserInfo);
}

void Friends::refresh()
{
    m_pMovable->removeAllChildrenWithCleanup(true);
    
    std::vector<VillageInfo *> v = m_pSocial->GetFollowing(m_pSystem->GetPlayer()->GetUserID(), 0);
    std::vector<VillageInfo *>::iterator i;
    int c = 0;

//    m_pSearch->removeAllContent(true);
    
    for (i = v.begin(); i != v.end(); ++i, ++c)
    {
        addFriendList((*i)->userID, c);
    
//        UserInfo *pUserInfo = _createUserInfo(m_pNetwork, (*i)->userID.data());
//        m_pSearch->pushContent(pUserInfo->userID.data(), pUserInfo);
    }
}

void Friends::refreshBySearch(std::string name)
{
    if (name.length() == 0)
    {
        refresh();
        return;
    }
    
    m_pMovable->removeAllChildrenWithCleanup(true);
    
    VillageInfo info;
    if (m_pSocial->FindVillageInfo(name.c_str(), &info))
    {
        
        addFriendList(info.userID);
        
//      UserInfo * uinfo = _createUserInfo(m_pNetwork, info.userID.c_str());
//      m_pSearch->removeContent(info.userID.c_str(), true);
//      m_pSearch->pushContent(info.userID.c_str(), uinfo);
    }
}

void Friends::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    TextFieldKeyBoardOn(this, pTouches, &m_pTextField);
    
    CCTouch * touch = static_cast<CCTouch *>(*pTouches->begin());
    CCPoint p = touch->locationInView();
    p = CCDirector::sharedDirector()->convertToGL(p);
    
    if (290 - p.y < 0)
        return;
    
    int n = (290 - p.y) / 60;
    
    CCNode * t = m_pMovable->getChildByTag(n);
    if (!t)
        return;
    
    CCLabelTTF * l = static_cast<CCLabelTTF *>(t->getChildByTag(0));
    const char * name = l->getString();

    if (m_pSystem->GetPlayer()->addFollowing(name, m_pNetwork))
    {
        CCMessageBox("Add Friend Success!", "Add Friend");
        return;
    }
    
    UserInfo uInfo;
    m_pSocial->FindUser(name, &uInfo, USER_SEARCH_ENUM_ID);
    EnterFriendVillage(&uInfo);
}

bool Friends::onTextFieldInsertText(cocos2d::CCTextFieldTTF * sender, const char * text, int nLen)
{
    if (text[0] == '\n')
        refreshBySearch(sender->getString());
        
    return false;
}

UserInfo* Friends::_createUserInfo(Network *pNetwork, const char *pUserName)
{
    const char *baseURL = "http://swmaestros-sng.appspot.com/searchmember?id=%s";
    char url[256];
    
    sprintf(url, baseURL, pUserName);
    
    CURL_DATA data;
    if(pNetwork->connectHttp(url, &data) != CURLE_OK)
        return NULL;
    
    UserInfo *pUserInfo = new UserInfo;

    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pNode = xmlDoc.first_node()->first_node();
    int count = atoi(pNode->value());
    
    if(count == 0)
        return NULL;
    
    pNode = pNode->next_sibling()->first_node();
    
    pUserInfo->userID = pNode->value();
    pNode = pNode->next_sibling();
    pUserInfo->userPhone = pNode->value();
    pNode = pNode->next_sibling();
    pUserInfo->userDate = pNode->value();
    
    return pUserInfo;
}

bool Friends::EnterFriendVillage(UserInfo *pUserInfo)
{
    SAFE_DELETE(m_pFriendVillage);
    m_pFriendVillage = new FriendVillage(1, pUserInfo, m_pNetwork, m_pSystem->GetPlayer()->GetUserID());
    
    if(m_pFriendVillage->init() == false)
        return false;
    
    Map * map = new Map(7);
    
    if (!map->init(m_pSystem, m_pFriendVillage))
        return false;
    
    m_pScene->Visit(map);
    /*
     여기서 띄우는 작업 해야하거나 할듯..
     */
    
    return true;
}