//
//  TypeDocument.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 19..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "BuildingInfo.h"
#include "CropInfo.h"
#include "OrnamentInfo.h"

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