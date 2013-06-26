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
    
    m_pStatement = NULL;
}

Sqlite3Base::~Sqlite3Base()
{
    sqlite3_close(m_pDataBase);
    m_pDataBase = NULL;
}

bool Sqlite3Base::ErrorCheck(const char *table, char *type, char *bind)
{
    char cmd[64];
    sprintf(cmd, "SELECT * FROM %s WHERE %s = ?", table, type);
    printf("%s \n", cmd);
    
    int errorMsg = sqlite3_prepare_v2(m_pDataBase, cmd, -1, &m_pStatement, NULL);
    
    if( errorMsg != SQLITE_OK )
    {
        printf("%s <- db error, error msg : %d\n", __FUNCTION__, errorMsg );
        
        return false;
    }
    else
    {
        if( strcmp(type, "name") == 0 )
            sqlite3_bind_text(m_pStatement, 1, bind, -1, NULL);
        else {
            sqlite3_bind_int(m_pStatement, 1, atoi(bind));
        }
        
        if( sqlite3_step(m_pStatement) != SQLITE_ROW )
            return false;
    }
    
    return true;
}

bool Sqlite3Base::ErrorCheck(const char *cmd)
{
//    char cmd[64];
//    sprintf(cmd, "SELECT * FROM %s", table);
//    printf("%s \n", cmd);
    
    int errorMsg = sqlite3_prepare_v2(m_pDataBase, cmd, -1, &m_pStatement, NULL);
    
    if( errorMsg != SQLITE_OK )
    {
        printf("%s <- db error, error msg : %d\n", __FUNCTION__, errorMsg );
        return false;
    }
    else
    {
        if( sqlite3_step(m_pStatement) != SQLITE_ROW )
            return false;
    }
    
    return true;
}

int Sqlite3Base::GetInt(int iCol)
{
    return sqlite3_column_int(m_pStatement, iCol);
}

const char* Sqlite3Base::GetTxt(int iCol)
{
    return (const char*)sqlite3_column_text(m_pStatement, iCol);
}

double Sqlite3Base::GetDouble(int iCol)
{
    return sqlite3_column_double(m_pStatement, iCol);
}

bool Sqlite3Base::isNextStep()
{
    return sqlite3_step(m_pStatement) == SQLITE_ROW;
}

void Sqlite3Base::Finalize()
{
    sqlite3_finalize(m_pStatement);
}
