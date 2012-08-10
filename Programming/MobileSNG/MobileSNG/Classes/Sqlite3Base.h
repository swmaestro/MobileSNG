//
//  Sqlite3Base.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 10..
//
//

#pragma once

#include <sqlite3.h>

class Sqlite3Base
{
protected:
    sqlite3     *m_pDataBase;
    
public:
    Sqlite3Base(const char *DBFileName);
    virtual ~Sqlite3Base();
    
protected:
    bool _errorCheck(const char *table, char *whereType, char *whereBind, sqlite3_stmt **ppStatement);
    
    bool _errorCheck(const char *table, sqlite3_stmt **ppStatement);
};