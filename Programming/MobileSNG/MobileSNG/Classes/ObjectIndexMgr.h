//
//  ObjectIndex.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 28..
//
//

#pragma once

#define OBJECT_INDEX_FILE_NAME "ObjectIndex.txt"

#include <vector>

class ObjectIndexMgr
{
private:
    bool         *m_pBuildingTable;
    
public:
    ObjectIndexMgr();
    ~ObjectIndexMgr();

private:
    int _indexGenerate(bool *pTable,int start = 0, int max = 1000);
    bool _addIndex(bool *pTable, int index);
    
public:
    int buildingIndex();
    int fieldIndex();
    int cropIndex();
    
public:
    bool addBuildingIndex(int idx);
    bool addFieldIndex(int idx);
    
public:
    bool removeBuildIndex(int idx);
    bool removeFieldIndex(int idx);
    
public:
    std::vector<int> emptyBuildingElement();
    std::vector<int> emptyFieldElement();
};