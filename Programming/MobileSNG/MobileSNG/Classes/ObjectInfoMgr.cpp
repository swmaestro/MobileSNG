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

ObjectInfoMgr::ObjectInfoMgr(const char *DBFileName)
{
    std::string pathKey = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(DBFileName);
    
    printf("database location : %s\n", pathKey.data());
  
    if( sqlite3_open(pathKey.data(), &m_pDataBase) != SQLITE_OK)
    {
        printf("%s database file error\n", __FUNCTION__ );
        sqlite3_close(m_pDataBase);
    }
}

ObjectInfoMgr::~ObjectInfoMgr()
{
    sqlite3_close(m_pDataBase);
    m_pDataBase = NULL;
}

bool ObjectInfoMgr::_errorCheck(const char *table, char *where, sqlite3_stmt **ppStatement)
{
    char cmd[64];
    sprintf(cmd, "SELECT * FROM %s WHERE %s", table, where);
    
    int errorMsg = sqlite3_prepare_v2(m_pDataBase, cmd, -1, ppStatement, NULL);
        
    if( errorMsg != SQLITE_OK )
    {
        printf("%s <- db error, error msg : %d\n", __FUNCTION__, errorMsg );
        
        return false;
    }
    else
    {
        if( sqlite3_step((*ppStatement)) != SQLITE_ROW )
            return false;
    }
    
    return true;
}

BUILDING_INFO ObjectInfoMgr::_searchBuildInfo(char *condition)
{
    sqlite3_stmt *statement;
    
    BUILDING_INFO info;
    
    if( _errorCheck("Building", condition, &statement) == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return info;
    }
    
    info.objectID    = sqlite3_column_int(statement, 0);
    info.price       = sqlite3_column_int(statement, 1);
    info.time        = sqlite3_column_int(statement, 2);
    info.reward      = sqlite3_column_int(statement, 3);
    
    info.name        = (char*)sqlite3_column_text(statement, 4);

    
    sqlite3_finalize(statement);
    
    return info;
}

CROP_INFO ObjectInfoMgr::_searchCropInfo(char *condition)
{
    sqlite3_stmt *statement = NULL;
    
    CROP_INFO info;
    
    if( _errorCheck("Crop", condition, &statement) == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return info;
    }
    
    info.objectID    = sqlite3_column_int(statement, 0);
    info.price       = sqlite3_column_int(statement, 1);
    info.time        = sqlite3_column_int(statement, 2);
    info.reward      = sqlite3_column_int(statement, 3);
    
    info.name        = (char*)sqlite3_column_text(statement, 4);
        
    sqlite3_finalize(statement);
    
    return info;
}

ORNAMENT_INFO ObjectInfoMgr::_searchOrnamentInfo(char *condition)
{
    sqlite3_stmt *statement;
    ORNAMENT_INFO info;
    
    if( _errorCheck("Ornament", condition, &statement) == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return info;
    }
    
    info.objectID    = sqlite3_column_int(statement, 0);
    info.price       = sqlite3_column_int(statement, 1);
    
    info.name        = (char*)sqlite3_column_text(statement, 4);
    
    sqlite3_finalize(statement);
    
    return info;
}

BUILDING_INFO ObjectInfoMgr::searchBuildInfo(int id)
{
    char condition[8];
    sprintf(condition, "id = %d", id);
    return _searchBuildInfo(condition);
}

BUILDING_INFO ObjectInfoMgr::searchBuildInfo(const char * name)
{
    char condition[8];
    sprintf(condition, "name = '%s'", name);
    return _searchBuildInfo(condition);
}

CROP_INFO ObjectInfoMgr::searchCropInfo(int id)
{
    char condition[8];
    sprintf(condition, "id = %d", id);
    return _searchCropInfo(condition);
}

CROP_INFO ObjectInfoMgr::searchCropInfo(const char * name)
{
    char condition[8];
    sprintf(condition, "name = '%s'", name);
    
    return _searchCropInfo(condition);
}

ORNAMENT_INFO ObjectInfoMgr::searchOrnamentInfo(int id)
{
    char condition[8];
    sprintf(condition, "id = %d", id);
    return _searchOrnamentInfo(condition);
}

ORNAMENT_INFO ObjectInfoMgr::searchOrnamentInfo(const char * name)
{
    char condition[8];
    sprintf(condition, "name = '%s'", name);
    return _searchOrnamentInfo(condition);
}
