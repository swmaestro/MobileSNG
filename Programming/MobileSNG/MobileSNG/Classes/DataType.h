//
//  TypeDocument.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 19..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <string>

template<typename TYPE>
struct POINT
{
    TYPE x, y;
    
    POINT(TYPE _x = 0, TYPE _y = 0)
    {
        x = _x;
        y = _y;
    }
};

template<typename TYPE>
struct SIZE
{
    TYPE width, height;
    
    SIZE(TYPE _w = 0, TYPE _h = 0)
    {
        width   = _w;
        height  = _h;
    }
};

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
    int             level;
    
    COMMON_INFO()
    {
        objectID = 0;
        price = 0;
    }
    
    COMMON_INFO(int _objectID, const char *_name, int _price, int _level)
    {
        objectID    = _objectID;
        price       = _price;
        
        name = _name;
        
        level = _level;
    }
    
    ~COMMON_INFO()
    {
        name.clear();
    }
};

struct BUILDING_INFO : public COMMON_INFO
{
    SIZE<int>       size;
    OBJECT_INFO     object;
    int             buildTime;
    
    BUILDING_INFO() : COMMON_INFO() { }
    BUILDING_INFO(OBJECT_INFO objectInfo, COMMON_INFO commonInfo, SIZE<int> buildingSize, int buildTime) :  COMMON_INFO(commonInfo)
    {
        size            = buildingSize;
        object          = objectInfo;
        this->buildTime = buildTime;
    }
};

struct CROP_INFO : public COMMON_INFO
{
    OBJECT_INFO object;
    
    CROP_INFO() : COMMON_INFO() { }
    CROP_INFO(OBJECT_INFO objectInfo, COMMON_INFO commonInfo) : COMMON_INFO(commonInfo)
    {
        object = objectInfo;
    }
};

struct ORNAMENT_INFO : public COMMON_INFO
{
    ORNAMENT_INFO() : COMMON_INFO(){}
    ORNAMENT_INFO(int _objectID, int _price, const char *_name, int _level) 
                         :COMMON_INFO(_objectID, _name, _price, _level){}
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