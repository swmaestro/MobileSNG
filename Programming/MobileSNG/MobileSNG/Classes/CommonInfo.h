//
//  CommonType.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 17..
//
//

#pragma once

#include <string>

class CommonInfo
{
private:
    int             m_systemVersion;
    int             m_objectID;
    int             m_price;
    int             m_cash;
    std::string     m_name;
    int             m_level;
    
public:
    CommonInfo()
    {
        m_objectID = 0;
        m_price = 0;
    }
    
    CommonInfo(int _objectID, const char *_name, int _price, int _level, int _cash, int _systemVersion)
    {
        m_objectID    = _objectID;
        m_price       = _price;
        
        m_name = _name;
        m_level = _level;
        
        m_systemVersion = _systemVersion;
        
        m_cash = _cash;
    }
    
public:
   inline int GetSystemVersion()  { return m_systemVersion;   }
   inline int GetObjectID()       { return m_objectID;        }
   inline int GetPrice()          { return m_price;           }
   inline int GetLevel()          { return m_level;           }
    
    inline std::string GetName()    { return m_name;            }
    inline int GetCash()            { return m_cash;        }
};