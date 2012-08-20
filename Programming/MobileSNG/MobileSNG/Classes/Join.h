//
//  Join.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 15..
//
//

#include "cocos2d.h"
#include "Network.h"
#include "DataType.h"

class JoinUI;

class Join
{
private:
    Network         *m_pNetwork;
    JoinUI          *m_pUI;
    
public:
    Join(Network *pNetwork, JoinUI *pUI);
    Join();
    
public:
    bool CreatAccount();
    bool Overlab();
    
private:
    bool _CheckPhoneNumber(const char *strPhone);
    
private:
    bool _Join(const char *strID, const char *strPassword, const char *strPhone);
    
    bool _CheckOverlapID(const char *strID);

public:
    bool isDone();
};