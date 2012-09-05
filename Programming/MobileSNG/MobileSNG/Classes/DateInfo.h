//
//  DateInfo.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 29..
//
//

#pragma once

#include <stdio.h>
#include <string>
#include <time.h>
    
class DateInfo
{
private:
    int m_year;
    int m_month;
    int m_day;
    int m_hour;
    int m_min;
    int m_sec;
    
public:
    DateInfo(int year = 0, int month = 0, int day = 0, int hour = 0, int min = 0, int sec = 0)
    {
        m_year      = year;
        m_month     = month;
        m_day       = day;
        m_hour      = hour;
        m_min       = min;
        m_sec       = sec;
    }
    ~DateInfo(){}
    
private:
    int _getInt(char *pText, int n)
    {
        int res = 0;
        
        for(int i=0; i<n; ++i)
        {
            int value = pText[i] - '0';
            if(value < 0 || value > 9) return -1;
            
            res += value * pow(10.f, n-i-1);
        }
        
        return res;
    }
    
public:
    bool UpdateDate(const char *pDate)
    {
        char *pText = const_cast<char*>(pDate);
        
        if( (m_year     = _getInt(pText, 4))    == -1)      return false;
        if( (m_month    = _getInt(pText+=5, 2)) == -1)      return false;
        if( (m_day      = _getInt(pText+=3, 2)) == -1)      return false;
        if( (m_hour     = _getInt(pText+=3, 2)) == -1)      return false;
        if( (m_min      = _getInt(pText+=3, 2)) == -1)      return false;
        if( (m_sec      = _getInt(pText+=3, 2)) == -1)      return false;
        
        return true;
    }
    
    long long int GetTimeValue(DateInfo &otherDate)
    {
        tm other;
        tm date;
        
        memset(&other, 0, sizeof(tm));
        memset(&date, 0, sizeof(tm));

        time_t t1;// = mktime(other);
        time_t t2;// = mktime(date);
    
        other.tm_year   = otherDate.m_year - 1900;
        other.tm_mon    = otherDate.m_month - 1;
        other.tm_mday   = otherDate.m_day;
        other.tm_hour   = otherDate.m_hour;
        other.tm_min    = otherDate.m_min;
        other.tm_sec    = otherDate.m_sec;
        other.tm_isdst  = 0;
        
        t1 = mktime(&other);

        date.tm_year   = m_year  - 1900;
        date.tm_mon    = m_month - 1;
        date.tm_mday   = m_day;
        date.tm_hour   = m_hour;
        date.tm_min    = m_min;
        date.tm_sec    = m_sec;
        date.tm_isdst = 0;
        
        t2 = mktime(&date);
                
        return (long long int)difftime(t1, t2);
    }
    
    
public:
    inline int GetYear()    { return m_year;    }
    inline int GetMonth()   { return m_month;   }
    inline int GetDay()     { return m_day;     }
    
    inline int GetHour()    { return m_hour;    }
    inline int GetMin()     { return m_min;     }
    inline int GetSec()     { return m_sec;     }
};
