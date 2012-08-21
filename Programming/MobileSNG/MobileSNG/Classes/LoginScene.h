//
//  LoginScene.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 14..
//
//

#include "cocos2d.h"
#include "Join.h"
#include "JoinUI.h"
#include "Login.h"
#include "LoginUI.h"
#include "Network.h"

class LoginScene : public cocos2d::CCLayer
{
private:
    cocos2d::CCSprite   *m_pBackGround;
    
    Join            *m_pJoin;
    JoinUI          *m_pJoinUI;
  
    Login           *m_pLogin;
    LoginUI         *m_pLoginUI;

    Network         *m_pNet;
    
        
public:
    LoginScene();
    ~LoginScene();
    
public:
    bool init();
    LAYER_CREATE_FUNC(LoginScene);
    
    static cocos2d::CCScene* scene();
    
private:
    bool _GetUserInfo(char *userID, char *outID, char *outPhone);
    
private:
    void _btOverlab(CCObject *pSender);
    void _btJoin(CCObject *pSender);
    void _btCancel(CCObject *pSender);
    
private:
    void _btLogin(CCObject *pSender);
    void _btChangeUI(CCObject *pSender);
    
private:
    void _ChangeUI(CCObject *pSender);
    void _Dealloc(CCObject *pSender);
    void _NextScene();
};