//
//  Join.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 15..
//
//

#include "cocos2d.h"
#include "Network.h"

class JoinUI;

class Join
{
private:
    Network         *m_pNetwork;
    
public:
    Join(Network *pNetwork);
    Join();
    
private:
    bool _CheckPhoneNumber(const char *strPhone);
    
public:
    bool CreatAccount(const char *strID, const char *strPassword, const char *strPhone, const int initGold = 2000, const int initCash = 1000);
    
    bool CheckOverlapID(const char *strID);

public:
    bool isDone();
};