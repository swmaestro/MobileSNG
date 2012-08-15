//
//  SQLiteiOS.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 19..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "sqlite/sqlite3.h"
#include "DataType.h"
#include <vector>
#include "Sqlite3Base.h"

class ObjectInfoMgr : private Sqlite3Base
{
public:
    ObjectInfoMgr(const char *DBFileName);
    ~ObjectInfoMgr();

private:
    int         _getTime(sqlite3_stmt *pStatement, int col);
    
private:
    bool        _searchInfo(const char *type, char *bind, BUILDING_INFO *pInfo); 
    bool        _searchInfo(const char *type, char *bind, CROP_INFO *pInfo); 
    bool        _searchInfo(const char *type, char *bind, ORNAMENT_INFO *pInfo); 
        
public:
    bool        searchInfo(int id, BUILDING_INFO *pInfo);
    bool        searchInfo(const char *name, BUILDING_INFO *pInfo);
     
    bool        searchInfo(int id, CROP_INFO *pInfo);
    bool        searchInfo(const char *name, CROP_INFO *pInfo);

    bool        searchInfo(int id, ORNAMENT_INFO *pInfo);
    bool        searchInfo(const char *name, ORNAMENT_INFO *pInfo);
    
public:
    std::vector<BUILDING_INFO>  GetAllBuildingInfo();
    std::vector<CROP_INFO>      GetAllCropInfo();
    std::vector<ORNAMENT_INFO>  GetAllOrnamentInfo();
};