//
//  Sqlite3Base.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 10..
//
//

#pragma once

#include "sqlite/sqlite3.h"

class Sqlite3Base
{
protected:
    sqlite3             *m_pDataBase;
    sqlite3_stmt        *m_pStatement;
    
public:
    Sqlite3Base(const char *DBFileName);
    virtual ~Sqlite3Base();
    
public:
    bool ErrorCheck(const char *table, char *whereType, char *whereBind);
    bool ErrorCheck(const char *cmd);
    
    bool isNextStep();
    void Finalize();
    
public:
    int             GetInt(int iCol);
    const char*     GetTxt(int iCol);
    double          GetDouble(int iCol);
};
