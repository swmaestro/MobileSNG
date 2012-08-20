//
//  FirstScene.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 14..
//
//

#include "FirstScene.h"
#include "User.h"
#include "GameScene.h"

using namespace cocos2d;

FirstScene::FirstScene()
{
    m_pJoin     = NULL;
    m_pUI       = NULL;
    m_pNet      = NULL;
}

FirstScene::~FirstScene()
{
}

CCScene* FirstScene::scene()
{
    CCScene * scene = CCScene::create();
    scene->addChild(FirstScene::create());
    return scene;
}

bool FirstScene::init()
{
    //user를 생성해야는데.. 끙..
    //User클래스에 스태틱으로 하나두고 파일이 있는지 없는지만 체크하는걸 하나 만들어두자
    //다음씬 넘어가는건 어떻게 하려구?
    
    if( CCLayer::init() == false )
        return false;
    
    if(User::hasFile())
    {
        CCCallFunc *p = CCCallFunc::create(this, callfunc_selector(FirstScene::_NextScene));
        runAction(p);
        return true;
    }
    
    m_pUI = new JoinUI("HelloWorld.png", this, menu_selector(FirstScene::_btJoin), NULL, menu_selector(FirstScene::_btOverlab));
    
    m_pUI->setAnchorPoint(ccp(0.0f,0.0f));
    m_pUI->setPosition(ccp(0,0));
    m_pNet = new Network;
    
    m_pJoin = new Join(m_pNet, m_pUI);

    addChild(m_pUI);
    
    return true;
}

void FirstScene::_btJoin(CCObject *pSender)
{
    CCLOG(__FUNCTION__);

    if(m_pJoin->CreatAccount())
    {
        CCMessageBox("Join OK", "Join OK");
        
        const char *id = m_pUI->GetContext(JOIN_UI_ENUM_ID);
        const char *pw = m_pUI->GetContext(JOIN_UI_ENUM_PW);
        const char *ph = m_pUI->GetContext(JOIN_UI_ENUM_PHONE);
        
        User::newUser(id, pw, ph);
        
        CCCallFunc *p = CCCallFunc::create(this, callfunc_selector(FirstScene::_NextScene));
        runAction(p);
    }
    else
    {
        CCMessageBox("Fail", "Fail");
        for(int i=0; i<JOIN_UI_ENUM_NUM; ++i)
            m_pUI->setEmptyTextField((JOIN_UI_ENUM)i);
    }
}

void FirstScene::_btOverlab(CCObject *pSender)
{
    CCLOG(__FUNCTION__);
    if(m_pJoin->Overlab())
        CCMessageBox("Overlab", "Overlab");
}

void FirstScene::_NextScene(CCObject *pSender)
{
    removeAllChildrenWithCleanup(true);
    
    SAFE_DELETE(m_pJoin);
    SAFE_DELETE(m_pUI);
    SAFE_DELETE(m_pNet);
    
    CCDirector::sharedDirector()->pushScene(GameScene::scene());
}