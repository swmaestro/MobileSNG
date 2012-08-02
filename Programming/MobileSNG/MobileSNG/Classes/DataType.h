//
//  TypeDocument.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 19..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <string>

struct OBJECT_INFO
{
    int time;
    int reward;
    
    OBJECT_INFO()
    {
        time = 0;
        reward = 0;
    }
    
    OBJECT_INFO(int _time, int _reward)
    {
        time        = _time;
        reward      = _reward;
    }
};

struct COMMON_INFO
{
    int             objectID;
    int             price;
    std::string     name;
    
    COMMON_INFO()
    {
        objectID = 0;
        price = 0;
    }
    
    COMMON_INFO(int _objectID, const char *_name, int _price)
    {
        objectID    = _objectID;
        price       = _price;
        
        name = _name;
    }
    
    ~COMMON_INFO()
    {
        name.clear();
    }
};

struct BUILDING_INFO : public COMMON_INFO, public OBJECT_INFO
{
    BUILDING_INFO() : OBJECT_INFO(), COMMON_INFO() { }
    BUILDING_INFO(OBJECT_INFO objectInfo, COMMON_INFO commonInfo) : OBJECT_INFO(objectInfo), COMMON_INFO(commonInfo)
    {
    }
};

struct CROP_INFO : public COMMON_INFO, public OBJECT_INFO
{
    CROP_INFO() : OBJECT_INFO(), COMMON_INFO() { }
    
    CROP_INFO(OBJECT_INFO objectInfo, COMMON_INFO commonInfo) :
    OBJECT_INFO(objectInfo), COMMON_INFO(commonInfo)
    {
    
    }
};

struct ORNAMENT_INFO : public COMMON_INFO
{
    ORNAMENT_INFO() : COMMON_INFO()
    {
        
    }
    ORNAMENT_INFO(int _objectID, int _price, const char *_name) :
    COMMON_INFO(_objectID, _name, _price)
    {
        
    }
};

struct USER_INFO
{
    int level;
    int money;
    int cash;
    int exp;
    
    USER_INFO(int _level = 0, int _money = 0, int _cash = 0, int _exp = 0)
    {
        level   = _level;
        money   = _money;
        cash    = _cash;
        exp     = _exp;
    }
};

template<typename TYPE>
struct POINT
{
    TYPE x, y;
};

template<typename TYPE>
struct SIZE
{
    TYPE width, height;
};