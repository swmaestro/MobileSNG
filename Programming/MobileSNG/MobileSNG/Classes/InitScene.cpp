//
//  InitScene.cpp
//  MobileSNG
//
//  Created by 건우 강 on 12. 9. 11..
//
//

#include "InitScene.h"
#include "LoginScene.h"

using namespace cocos2d;

CCScene * InitScene::scene()
{
    CCScene * scene = CCScene::create();
    scene->addChild(InitScene::create());
    return scene;
}

bool InitScene::init()
{
    if (!CCLayer::create())
        return false;
    
    CCSprite * t = CCSprite::create("Logo-Background.png");
    t->setAnchorPoint(ccp(0, 0));
    t->setPosition(ccp(0, 0));
    addChild(t);
    
    return true;
}

void InitScene::onEnter()
{
    CCLayer::onEnter();
    
    CCScene * scene = LoginScene::scene();
    //CCDirector::sharedDirector()->popScene();
    CCDirector::sharedDirector()->pushScene(scene);
}