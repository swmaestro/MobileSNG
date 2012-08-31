//
//  SQLiteiOS.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 19..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "sqlite/sqlite3.h"
#include "BuildingInfo.h"
#include "OrnamentInfo.h"
#include "CropInfo.h"
#include <vector>
#include <map>
#include "Sqlite3Base.h"

/*
 이거 갈아 엎는다면 어떤 형식으로?
 초기 db에서 죄다 싹 읽어서, m_vBuilding, m_vCrop, m_vOrnament에 저장해
 그리고 원래 쓰던 SearchInfo들은 전부 싹 개조해서 갈아얖어야겠지 뭐
 */

class ObjectInfoMgr
{    
private:
    std::vector<CropInfo*>                 m_vCropInfo;
    std::vector<BuildingInfo*>             m_vBuildingInfo;
    std::vector<OrnamentInfo*>             m_vOrnamentInfo;
    
    std::map<const char*, CropInfo*>       m_cropInfoMap;
    std::map<const char*, BuildingInfo*>   m_buildingInfoMap;
    std::map<const char*, OrnamentInfo*>   m_ornamentInfoMap;
    
public:
    ObjectInfoMgr();
    ~ObjectInfoMgr();

public:
    bool        loadData(const char *DBFileName);
    
private:
    int         _getTime(int timeValue);
    
private:
    bool        _loadBuilding(Sqlite3Base *pDBBase);
    bool        _loadCrop(Sqlite3Base *pDBBase);
    bool        _loadOrnament(Sqlite3Base *pDBBase);
        
public:
    bool        searchInfo(int id, BuildingInfo **ppInfo);
    bool        searchInfo(const char *name, BuildingInfo **ppInfo);
     
    bool        searchInfo(int id, CropInfo **ppInfo);
    bool        searchInfo(const char *name, CropInfo **ppInfo);

    bool        searchInfo(int id, OrnamentInfo **ppInfo);
    bool        searchInfo(const char *name, OrnamentInfo **ppInfo);
    
public:
    std::vector<BuildingInfo*>  GetAllBuildingInfo();
    std::vector<CropInfo*>      GetAllCropInfo();
    std::vector<OrnamentInfo*>  GetAllOrnamentInfo();
};