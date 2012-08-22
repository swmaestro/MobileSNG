//
//  SQLiteiOS.m
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 19..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "ObjectInfoMgr.h"
#include <string>
#include "CCFileUtils.h"

using namespace cocos2d;
using namespace std;

ObjectInfoMgr::ObjectInfoMgr()
{
}

ObjectInfoMgr::~ObjectInfoMgr()
{
    int size;
    
    size = m_vCropInfo.size();
    
    for(int i=0; i<size; ++i)       delete m_vCropInfo[i];
    m_vCropInfo.clear();
    m_cropInfoMap.clear();
    
    size = m_vBuildingInfo.size();
    
    for(int i=0; i<size; ++i)       delete m_vBuildingInfo[i];
    m_vBuildingInfo.clear();
    m_buildingInfoMap.clear();
    
    size = m_vOrnamentInfo.size();
    
    for(int i=0; i<size; ++i)       delete m_vCropInfo[i];
    m_vCropInfo.clear();
    m_cropInfoMap.clear();
}

bool ObjectInfoMgr::_loadBuilding(Sqlite3Base *pDBBase)
{
    if( pDBBase->ErrorCheck("SELECT * FROM Building Order By id") == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return false;
    }
    do
    {
        int size                = pDBBase->GetInt(7);

        ObjectInfo  obj     (_getTime(pDBBase->GetInt(6)), pDBBase->GetInt(4), pDBBase->GetInt(9), pDBBase->GetInt(10));
        CommonInfo  common  (pDBBase->GetInt(1), pDBBase->GetTxt(2), pDBBase->GetInt(3), pDBBase->GetInt(8), pDBBase->GetInt(0));
        
        BuildingInfo *pInfo = new BuildingInfo(obj, common, SIZE<int>(size/100, size%100), _getTime(pDBBase->GetInt(5)));
        
        pair<const char*, BuildingInfo*> p;
        p.first     = pInfo->GetName().data();
        p.second    = pInfo;
        
        m_vBuildingInfo.push_back(pInfo);
        m_buildingInfoMap.insert(p);
    }
    while (pDBBase->isNextStep());
    pDBBase->Finalize();

    return true;
}

bool ObjectInfoMgr::_loadCrop(Sqlite3Base *pDBBase)
{
    if( pDBBase->ErrorCheck("SELECT * FROM Crop Order By id") == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return false;
    }
    
    do
    {
        ObjectInfo obj          (_getTime(pDBBase->GetInt(5)), pDBBase->GetInt(4), pDBBase->GetInt(7), pDBBase->GetInt(8));
        CommonInfo common       (pDBBase->GetInt(1), pDBBase->GetTxt(2), pDBBase->GetInt(3), pDBBase->GetInt(6), pDBBase->GetInt(0));
        
        
        CropInfo *pInfo = new CropInfo(obj, common);
                
        pair<const char*, CropInfo*> p;
        p.first     = pInfo->GetName().data();
        p.second    = pInfo;
        
        m_vCropInfo.push_back(pInfo);
        m_cropInfoMap.insert(p);
    }
    while (pDBBase->isNextStep());
    pDBBase->Finalize();
    
    return true;
}

bool ObjectInfoMgr::_loadOrnament(Sqlite3Base *pDBBase)
{
    
    
    if( pDBBase->ErrorCheck("SELECT * FROM Ornament Order By id") == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return true;
    }

    do
    {
        CommonInfo common(pDBBase->GetInt(1), pDBBase->GetTxt(2), pDBBase->GetInt(3), pDBBase->GetInt(4), pDBBase->GetInt(0));
        OrnamentInfo *pInfo = new OrnamentInfo(common);
        
        pair<const char*, OrnamentInfo*> p;
        
        p.first     = pInfo->GetName().data();
        p.second    = pInfo;
        
        m_vOrnamentInfo.push_back(pInfo);
        m_ornamentInfoMap.insert(p);
    }
    while (pDBBase->isNextStep());
    pDBBase->Finalize();
    
    return true;
}

bool ObjectInfoMgr::loadData(const char *DBFileName)
{
    Sqlite3Base *pDBBase = new Sqlite3Base(DBFileName);
    
    if(_loadBuilding(pDBBase) == false)
        printf("Error, Cant Load Building Data\n");
    if(_loadCrop(pDBBase) == false)
        printf("Error, Cant Load Building Data\n");
    if(_loadOrnament(pDBBase) == false)
        printf("Error, Cant Load Ornament Data\n");
    
    delete pDBBase;
    return true;
}

int ObjectInfoMgr::_getTime(int timeValue)
{
    int hour = timeValue/10000;
    int min = (timeValue - hour*10000)/100;
    int sec = timeValue%100;

    return sec + (min * 60) + (hour * 3600);
}

bool ObjectInfoMgr::searchInfo(int id, BuildingInfo **ppInfo)
{
    if( (id+1) > m_vBuildingInfo.size() ) return false;
    *ppInfo = m_vBuildingInfo[id+1];
    return true;
}

bool ObjectInfoMgr::searchInfo(const char * name, BuildingInfo **ppInfo)
{
    BuildingInfo *pOut = m_buildingInfoMap.find(name)->second;
    if( pOut == NULL)        return false;
    *ppInfo = pOut;
    
    return true;
}

bool ObjectInfoMgr::searchInfo(int id, CropInfo **ppInfo)
{
    if( id > m_vCropInfo.size() ) return false;
    *ppInfo = m_vCropInfo[id];
    return true;
}

bool ObjectInfoMgr::searchInfo(const char * name, CropInfo **ppInfo)
{
    CropInfo *pOut = m_cropInfoMap.find(name)->second;
    if(pOut == NULL) return false;
    *ppInfo = pOut;
    return true;
}

bool ObjectInfoMgr::searchInfo(int id, OrnamentInfo **ppInfo)
{
    if( id > m_vOrnamentInfo.size()) return false;
    *ppInfo = m_vOrnamentInfo[id];
    return true;
}

bool ObjectInfoMgr::searchInfo(const char * name, OrnamentInfo **ppInfo)
{
    OrnamentInfo *pOut = m_ornamentInfoMap.find(name)->second;
    if(pOut == NULL) return false;
    *ppInfo = pOut;
    return true;
}

vector<BuildingInfo*> ObjectInfoMgr::GetAllBuildingInfo()
{
    return m_vBuildingInfo;
}

vector<CropInfo*> ObjectInfoMgr::GetAllCropInfo()
{
    return m_vCropInfo;
}

vector<OrnamentInfo*> ObjectInfoMgr::GetAllOrnamentInfo()
{
    return m_vOrnamentInfo;
}