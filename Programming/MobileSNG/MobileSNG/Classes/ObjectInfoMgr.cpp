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

ObjectInfoMgr::ObjectInfoMgr(const char *DBFileName) : Sqlite3Base(DBFileName)
{
}

ObjectInfoMgr::~ObjectInfoMgr()
{
}

int ObjectInfoMgr::_getTime(sqlite3_stmt *pStatement, int col)
{
    int dbTimeValue = sqlite3_column_int(pStatement, col);
    
    int hour = dbTimeValue/10000;
    int min = (dbTimeValue - hour*10000)/100;
    int sec = dbTimeValue%100;

    return sec + (min * 60) + (hour * 3600);
}

bool ObjectInfoMgr::_searchInfo(const char *type, char *bind, BUILDING_INFO *pInfo)
{
    sqlite3_stmt *statement;
    
    BUILDING_INFO info;
    
    if( _errorCheck("Building", const_cast<char*>(type), bind, &statement) == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return false;
    }
        
    info.objectID           = sqlite3_column_int(statement, 0);
    info.price              = sqlite3_column_int(statement, 1);
    info.object.time        = _getTime(statement, 2);
    info.object.reward      = sqlite3_column_int(statement, 3);
    
    int size                = sqlite3_column_int(statement, 4);

    info.size.width         = size/100;
    info.size.height        = size%100;
    
    info.level              = sqlite3_column_int(statement, 5);
    info.name               = (char*)sqlite3_column_text(statement, 6);
    info.buildTime          = _getTime(statement, 7);
    
    *pInfo = info;
    
    sqlite3_finalize(statement);
    
    return true;
}

bool ObjectInfoMgr::_searchInfo(const char *type, char *bind, CROP_INFO *pInfo)
{
    sqlite3_stmt *statement = NULL;
    
    CROP_INFO info;
    
    if( _errorCheck("Crop", const_cast<char*>(type), bind, &statement) == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return false;
    }
        
    info.objectID           = sqlite3_column_int(statement, 0);
    info.price              = sqlite3_column_int(statement, 1);
    
    info.object.time        = _getTime(statement, 2);

    info.object.reward      = sqlite3_column_int(statement, 3);
    
    info.level       = sqlite3_column_int(statement, 4);
    info.name        = (char*)sqlite3_column_text(statement, 5);
        
    sqlite3_finalize(statement);
    
    *pInfo = info;
    
    return true;
}

bool ObjectInfoMgr::_searchInfo(const char *type, char *bind, ORNAMENT_INFO *pInfo)
{
    sqlite3_stmt *statement;
    ORNAMENT_INFO info;
        
    if( _errorCheck("Ornament", const_cast<char*>(type), bind, &statement) == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return false;
    }
    
    info.objectID    = sqlite3_column_int(statement, 0);
    info.price       = sqlite3_column_int(statement, 1);
    
    info.level       = sqlite3_column_int(statement, 2);
    info.name        = (char*)sqlite3_column_text(statement, 3);
    
    sqlite3_finalize(statement);
    
    *pInfo = info;
    
    return true;
}

bool ObjectInfoMgr::searchInfo(int id, BUILDING_INFO *pInfo)
{
    char num[8];
    sprintf(num, "%d", id);
    return _searchInfo("id", num, pInfo);
}

bool ObjectInfoMgr::searchInfo(const char * name, BUILDING_INFO *pInfo)
{
    return _searchInfo("name", const_cast<char*>(name), pInfo);
}

bool ObjectInfoMgr::searchInfo(int id, CROP_INFO *pInfo)
{
    char num[8];
    sprintf(num, "%d", id);
    return _searchInfo("id", num, pInfo);
}

bool ObjectInfoMgr::searchInfo(const char * name, CROP_INFO *pInfo)
{
    return _searchInfo("name", const_cast<char*>(name), pInfo);
}

bool ObjectInfoMgr::searchInfo(int id, ORNAMENT_INFO *pInfo)
{
    char num[8];
    sprintf(num, "%d", id);
    return _searchInfo("id", num, pInfo);
}

bool ObjectInfoMgr::searchInfo(const char * name, ORNAMENT_INFO *pInfo)
{
    return _searchInfo("name", const_cast<char*>(name), pInfo);
}

vector<BUILDING_INFO> ObjectInfoMgr::GetAllBuildingInfo()
{
    sqlite3_stmt *pStatement;
    vector<BUILDING_INFO> vBuilding;
    BUILDING_INFO info;
    
    if( _errorCheck("Building Order by id", &pStatement) == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return vBuilding;
    }
    
    //120812 CA Edited While to Do~While
    do
    {
        info.objectID           = sqlite3_column_int(pStatement, 0);
        info.price              = sqlite3_column_int(pStatement, 1);
        
        info.object.time        = _getTime(pStatement, 2);
        info.object.reward      = sqlite3_column_int(pStatement, 3);
        
        int size;
        
        size                    = sqlite3_column_int(pStatement, 4);
        info.level              = sqlite3_column_int(pStatement, 5);
        info.name               = (char*)sqlite3_column_text(pStatement, 6);
        
        info.size.width         = size/100;
        info.size.height        = size%100;
        
        vBuilding.push_back(info);
    }
    while (sqlite3_step(pStatement) == SQLITE_ROW);
    
    sqlite3_finalize(pStatement);
    
    
    return vBuilding;
}

vector<CROP_INFO> ObjectInfoMgr::GetAllCropInfo()
{
    sqlite3_stmt        *pStatement;
    vector<CROP_INFO>   vCrop;
    CROP_INFO           info;
    
    if( _errorCheck("Crop Order by id", &pStatement) == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return vCrop;
    }
    
    //120812 CA Edited While to Do~While
    do
    {
        info.objectID           = sqlite3_column_int(pStatement, 0);
        info.price              = sqlite3_column_int(pStatement, 1);
        info.object.time        = _getTime(pStatement, 2);
        info.object.reward      = sqlite3_column_int(pStatement, 3);
        
        info.level              = sqlite3_column_int(pStatement, 4);
        info.name               = (char*)sqlite3_column_text(pStatement, 5);
        
        vCrop.push_back(info);
    }
    while (sqlite3_step(pStatement) == SQLITE_ROW);
    
    sqlite3_finalize(pStatement);
    
    
    return vCrop;
}

vector<ORNAMENT_INFO> ObjectInfoMgr::GetAllOrnamentInfo()
{
    sqlite3_stmt            *pStatement;
    vector<ORNAMENT_INFO>   vOrnament;
    ORNAMENT_INFO           info;
    
    if( _errorCheck("Ornament Order by id", &pStatement) == false )
    {
        printf("%s <- Error \n", __FUNCTION__);
        return vOrnament;
    }
    
    //120812 CA Edited While to Do~While
    do
    {
        info.objectID    = sqlite3_column_int(pStatement, 0);
        info.price       = sqlite3_column_int(pStatement, 1);
        
        info.level       = sqlite3_column_int(pStatement, 2);
        info.name        = (char*)sqlite3_column_text(pStatement, 3);
        
        vOrnament.push_back(info);
    }
    while (sqlite3_step(pStatement) == SQLITE_ROW);
    
    sqlite3_finalize(pStatement);
    
    return vOrnament;
}