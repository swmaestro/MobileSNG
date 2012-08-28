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
    bool         *m_pCropTable;
    
public:
    ObjectIndexMgr();
    ~ObjectIndexMgr();

private:
    int _indexGenerate(bool *pTable);
    bool _addIndex(bool *pTable, int index);
    
public:
    int buildingIndex();
    int cropIndex();
    
public:
    bool addBuildingIndex(int idx);
    bool addCropIndex(int idx);
    
public:
    void removeBuildIndex(int idx);
    void removeCropIndex(int idx);
    
public:
    std::vector<int> emptyBuildingElement();
    std::vector<int> emptyCropElement();
};