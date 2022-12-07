#include "Mysql.h"



int CMysql::ConnectMysql(const char* server,const char* user,const char* password, const char* database)
{
    conn = NULL;
    conn = mysql_init(NULL);
    mysql_set_character_set(conn,"utf8");
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        return FALSE;
    }
    return TRUE;
}

int CMysql::SelectMysql(char* szSql,int nColumn,list<string>& lst)
{
    MYSQL_RES * results = NULL;
    if(mysql_query(conn,szSql))return FALSE;
    results = mysql_store_result(conn);
    if(NULL == results)return FALSE;
    MYSQL_ROW record;
    while((record = mysql_fetch_row(results)))
    {
        for(int i=0; i<nColumn; i++)
        {
            lst.push_back( record[i] );
       //     q_Push(pQueue,(void*)record[i]);
        }
    }
    return TRUE;
}


int CMysql::UpdataMysql(char *szsql)
{
    if(!szsql)return FALSE;
    if(mysql_query(conn,szsql))return FALSE;

    return TRUE;
}

void CMysql::DisConnect()
{
    mysql_close(conn);
}
