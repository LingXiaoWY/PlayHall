#ifndef _MYSQL_H
#define _MYSQL_H
#include "packdef.h"
#include <mysql/mysql.h>
#include<list>
#include<string>

using namespace  std;


class CMysql{
public:
    int ConnectMysql(const char *server, const char *user, const char *password, const char *database);
    int SelectMysql(char* szSql,int nColumn,list<string>& lst);
    int UpdataMysql(char *szsql);
    void DisConnect();
private:
    MYSQL *conn;
//    MYSQL_RES *results;
//    MYSQL_ROW record;
};
#endif
