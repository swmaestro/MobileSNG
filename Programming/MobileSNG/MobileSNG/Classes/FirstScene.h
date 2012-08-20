//
//  FirstScene.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 14..
//
//

#include "cocos2d.h"
#include "Join.h"
#include "JoinUI.h"
#include "Network.h"

class FirstScene : public cocos2d::CCLayer
{
private:
    Join            *m_pJoin;
    JoinUI          *m_pUI;
    Network         *m_pNet;
        
public:
    FirstScene();
    ~FirstScene();
    
public:
    bool init();
    LAYER_CREATE_FUNC(FirstScene);
    
    static cocos2d::CCScene* scene();
    
private:
    void _btJoin(CCObject *pSender);
    void _btOverlab(CCObject *pSender);

private:
    void _NextScene(CCObject *pSender);
};