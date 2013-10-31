//
//  ObjectIndex.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 28..
//
//

#include "ObjectIndexMgr.h"
#include "CCFileUtils.h"
#include <string>

using namespace cocos2d;
using namespace std;

ObjectIndexMgr::ObjectIndexMgr()
{
    m_pBuildingTable    = new bool[2000];
    
    memset(m_pBuildingTable, 0, sizeof(bool)*2000);
}

ObjectIndexMgr::~ObjectIndexMgr()
{
    delete[] m_pBuildingTable;
    m_pBuildingTable = NULL;
}

int ObjectIndexMgr::_indexGenerate(bool *pTable, int start, int max)
{
    for(int idx = start; idx < max; ++idx)
    {
        if(pTable[idx] == false)
            return idx;
    }
    
    return -1;
}

bool ObjectIndexMgr::_addIndex(bool *pTable, int index)
{
    if( pTable[index] == false )
        pTable[index] = true;
    else return false;
    return true;
}

int ObjectIndexMgr::buildingIndex()
{
    return _indexGenerate(m_pBuildingTable);
}

int ObjectIndexMgr::fieldIndex()
{
    return _indexGenerate(m_pBuildingTable, 1000, 2000);
}

bool ObjectIndexMgr::addBuildingIndex(int idx)
{
    if(idx >= 1000) return false;
    return _addIndex(m_pBuildingTable, idx);
}

bool ObjectIndexMgr::addFieldIndex(int idx)
{
    if( idx < 1000 ) return false;
    return _addIndex(m_pBuildingTable, idx);
}

bool ObjectIndexMgr::removeBuildIndex(int idx)
{
    if( idx >= 1000) return false;
    m_pBuildingTable[idx] = false;
    return true;
}

bool ObjectIndexMgr::removeFieldIndex(int idx)
{
    if( idx < 1000) return false;
    m_pBuildingTable[idx] = false;
    return true;
}

std::vector<int> ObjectIndexMgr::emptyBuildingElement()
{
    vector<int> v;
    
    for(int i=0; i<1000; ++i)
    {
        if(m_pBuildingTable[i] == false)
            v.push_back(i);
    }
    
    return v;
}

std::vector<int> ObjectIndexMgr::emptyFieldElement()
{
    vector<int> v;
    
    for(int i=1000; i<2000; ++i)
    {
        if(m_pBuildingTable[i] == false)
            v.push_back(i);
    }
    
    return v;
}
