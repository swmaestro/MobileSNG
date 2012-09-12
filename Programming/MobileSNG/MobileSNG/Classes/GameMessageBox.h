//
//  MessageBox.h
//  MobileSNG
//
//  Created by 박 진 on 12. 9. 12..
//
//

#pragma once

#include "cocos2d.h"

enum GAMEMSGBOX_TAG
{
    GAMEMSGBOX_TAG_NONE,
    GAMEMSGBOX_TAG_ADD_OBJECT_ERROR = GAMEMSGBOX_TAG_NONE,
    GAMEMSGBOX_TAG_ADD_CROP_ERROR,
    GAMEMSGBOX_TAG_SELL_ERROR
};

class GameMessageBox : public cocos2d::CCLayer
{
private:
    cocos2d::CCSprite *m_pBackGround;
    cocos2d::CCSprite *m_pOK;
    
    cocos2d::CCLabelTTF *m_pGameMsg[3];
    
public:
    GameMessageBox();
    ~GameMessageBox();
    
public:
    bool init(cocos2d::CCTexture2D *pBackGround, cocos2d::CCTexture2D *pOK);
    bool btOKTouch(cocos2d::CCTouch *pTouch);
};