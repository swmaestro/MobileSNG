//
//  TypeDocument.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 19..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <string>

#pragma mark POSITION

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

template <typename TYPE>
inline bool intersectBoxWithBox(POINT<TYPE> pos1, SIZE<TYPE> size1,
                                POINT<TYPE> pos2, SIZE<TYPE> size2)
{
    if( pos2.x > pos1.x + size1.width )
        return false;
    if( pos2.x + size2.width < pos1.x )
        return false;
    if( pos2.y > pos1.y + size1.height )
        return false;
    if( pos2.y + size2.height < pos2.y )
        return false;
    
    return true;
}

template <typename TYPE>
inline bool intersectBoxWithPoint(POINT<TYPE> boxPos, SIZE<TYPE> boxSize, POINT<TYPE> pos)
{
    if( (boxPos.x <= pos.x && pos.x < boxPos.x + boxSize.width) &&
       (boxPos.y <= pos.y && pos.y < boxPos.y + boxSize.height) )
        return true;
    
    return false;
}

#pragma mark -
#pragma INFORMATION

struct OBJECT_INFO
{
    int time;
    int   reward;
    
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
    int             systemVersion;
    int             objectID;
    int             price;
    std::string     name;
    int             level;
    
    COMMON_INFO()
    {
        objectID = 0;
        price = 0;
    }
    
    COMMON_INFO(int _objectID, const char *_name, int _price, int _level, int _systemVersion)
    {
        objectID    = _objectID;
        price       = _price;

        name = _name;
        level = _level;

        systemVersion = _systemVersion;
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
    ORNAMENT_INFO(COMMON_INFO commonInfo) : COMMON_INFO(commonInfo){}
};

struct DATE {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    
    DATE(int year = 0, int month = 0, int day = 0, int hour = 0, int min = 0, int sec = 0)
    {
        this->year      = year;
        this->month     = month;
        this->day       = day;
        this->hour      = hour;
        this->min       = min;
        this->sec       = sec;
    }
};

struct USER_INFO {
    std::string strName;
    std::string strPhoneNumber;
    DATE        date;
    
    USER_INFO(){}
    USER_INFO(char *strName, char *strPhoneNumber, DATE date)
    {
        this->strName = strName;
        this->strPhoneNumber = strPhoneNumber;
        this->date = date;
    }
};