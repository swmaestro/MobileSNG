//
//  SQLiteiOS.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 19..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <sqlite3.h>
#include "DataType.h"


class ObjectInfoMgr
{
private:
    sqlite3     *m_pDataBase;
    
public:
    ObjectInfoMgr(const char *DBFileName);
    ~ObjectInfoMgr();

private:
    bool        _errorCheck(const char *table, char *where, sqlite3_stmt **ppStatement);
    
    BUILDING_INFO        _searchBuildInfo(char *condition); 
    CROP_INFO            _searchCropInfo(char *condition); 
    ORNAMENT_INFO        _searchOrnamentInfo(char *condition); 

    
public:
    BUILDING_INFO        searchBuildInfo(int id);
    BUILDING_INFO        searchBuildInfo(const char *name);
     
    CROP_INFO            searchCropInfo(int id);
    CROP_INFO            searchCropInfo(const char *name);

    ORNAMENT_INFO        searchOrnamentInfo(int id);
    ORNAMENT_INFO        searchOrnamentInfo(const char *name);
};