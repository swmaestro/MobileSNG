//
//  Sqlite3Base.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 10..
//
//

#include "Sqlite3Base.h"
#include <string>
#include "CCFileUtils.h"
#include <stdlib.h>

using namespace cocos2d;
using namespace std;

Sqlite3Base::Sqlite3Base(const char *DBFileName)
{
    std::string pathKey = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(DBFileName);
    
    printf("database location : %s\n", pathKey.data());
    
    if( sqlite3_open(pathKey.data(), &m_pDataBase) != SQLITE_OK)
    {
        printf("%s database file error\n", __FUNCTION__ );
        sqlite3_close(m_pDataBase);
    }
}

Sqlite3Base::~Sqlite3Base()
{
    sqlite3_close(m_pDataBase);
    m_pDataBase = NULL;
}

bool Sqlite3Base::_errorCheck(const char *table, char *type, char *bind, sqlite3_stmt **ppStatement)
{
    char cmd[64];
    sprintf(cmd, "SELECT * FROM %s WHERE %s = ?", table, type);
    printf("%s \n", cmd);
    
    int errorMsg = sqlite3_prepare_v2(m_pDataBase, cmd, -1, ppStatement, NULL);
    
    if( errorMsg != SQLITE_OK )
    {
        printf("%s <- db error, error msg : %d\n", __FUNCTION__, errorMsg );
        
        return false;
    }
    else
    {
        if( strcmp(type, "name") == 0 )
            sqlite3_bind_text((*ppStatement), 1, bind, -1, NULL);
        else {
            sqlite3_bind_int((*ppStatement), 1, atoi(bind));
        }
        
        if( sqlite3_step((*ppStatement)) != SQLITE_ROW )
            return false;
    }
    
    return true;
}

bool Sqlite3Base::_errorCheck(const char *table, sqlite3_stmt **ppStatement)
{
    char cmd[64];
    sprintf(cmd, "SELECT * FROM %s", table);
    printf("%s \n", cmd);
    
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
