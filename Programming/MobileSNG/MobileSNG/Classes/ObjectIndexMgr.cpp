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
    m_pBuildingTable    = new bool[1000];
    m_pCropTable        = new bool[1000];
    
    memset(m_pBuildingTable, 0, sizeof(bool)*1000);
    memset(m_pCropTable, 0, sizeof(bool)*1000);
}

ObjectIndexMgr::~ObjectIndexMgr()
{
    delete[] m_pBuildingTable;
    m_pBuildingTable = NULL;
    delete[] m_pCropTable;
    m_pCropTable = NULL;
}

int ObjectIndexMgr::_indexGenerate(bool *pTable)
{
    for(int idx = 0; idx < 1000; ++idx)
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

int ObjectIndexMgr::cropIndex()
{
    return _indexGenerate(m_pCropTable) + 1000;
}

bool ObjectIndexMgr::addBuildingIndex(int idx)
{
    if(idx >= 1000) return false;
    return _addIndex(m_pBuildingTable, idx);
}

bool ObjectIndexMgr::addCropIndex(int idx)
{
    if(idx < 1000) return false;
    idx -= 1000;
    
    return _addIndex(m_pCropTable, idx);
}

void ObjectIndexMgr::removeBuildIndex(int idx)
{
    m_pBuildingTable[idx] = false;
}

void ObjectIndexMgr::removeCropIndex(int idx)
{
    m_pCropTable[idx] = false;
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

std::vector<int> ObjectIndexMgr::emptyCropElement()
{
    vector<int> v;
    
    for(int i=0; i<1000; ++i)
    {
        if(m_pCropTable[i] == false)
            v.push_back(i+1000);
    }
    
    return v;
}