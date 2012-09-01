//
//  ListBoxSearch.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 26..
//
//

#pragma once

#include <pair.h>
#include <vector>
#include <algorithm>

using namespace std;

template<typename TContent>
class ListBoxSearch
{
private:
    vector< pair<const char*, TContent> > m_vContents;
    
public:
    ListBoxSearch(){}
    ~ListBoxSearch(){}
    
private:
    int _findObject(const char *key)
    {
        typename vector< pair<const char*, TContent> >::iterator iter;
        int i = 0;
        
        for(iter = m_vContents.begin(); iter != m_vContents.end(); ++iter, ++i)
        {
            if( strcmp((*iter).first,key) == 0)
                return i;
        }
        
        return -1;
    }
    
public:
    bool pushContent(const char *key, TContent content)
    {
        pair<const char*, TContent> pair(key, content);
        if(_findObject(key) != -1) return false;
        m_vContents.push_back(pair);
        return true;
    }
    
    bool addContent(const char *key, TContent content)
    {
        return pushContent(key, content);
    }
    
    bool removeContent(const char *key)
    {
        int i = _findObject(key);
        if( i == -1 )
            return false;
        
        m_vContents.erease(m_vContents.begin()+i);
        
        return true;
    }
    
public:
    vector<TContent> search(const char* key)
    {
        typename vector< pair<const char*, TContent> >::iterator iter;
        vector<TContent> v;
        
        for(iter = m_vContents.begin(); iter != m_vContents.end(); ++iter)
        {
            if(strstr((*iter).second, key))
                v.push_back((*iter).second);
        }
        
        sort(v.begin(), v.end());
        
        return v;
    }
    
    TContent GetContent(int index)
    {
        return m_vContents[index].second;
    }
    
public:
    size_t size()
    {   return m_vContents.size();  }
    
    bool empty()
    {   return m_vContents.empty(); }
    
};