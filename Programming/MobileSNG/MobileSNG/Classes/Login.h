//
//  Login.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 21..
//
//

#pragma once

#include "Network.h"

class Login
{
private:
    Network     *m_pNetwork;
    
public:
    Login(Network *pNetwork);
    ~Login();
    
public:
    bool Logon(const char *userID, const char *userPW);
};