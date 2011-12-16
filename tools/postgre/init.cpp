#include <stdio.h>
#include <string>
#include <iostream>

#include "libpq-fe.h"

using namespace std;

const string CR_TBL_SQL = "create table usergroup ("
                    "id varchar(80),"
                    "groups varchar(80)"
                    ")";
const string CR_INDX_SQL = "create index usergroup_id_index "
                    "on usergroup (id)";

int main()
{
    string conninfo = "host='localhost' port=5432 dbname='test'";

    PGconn *conn;

    conn = PQconnectdb(conninfo.c_str());
    if(conn && PQstatus(conn) != CONNECTION_BAD)
    {
        cout << "connected" << endl;
    } else
    {
        cout << "error happened" << endl;
        PQfinish(conn);
        return -1;
    }

    PGresult *result;

    result = PQexec(conn, CR_TBL_SQL.c_str());
    if(result && PQresultStatus(result) == PGRES_COMMAND_OK)
    {
        cout << "create table done" << endl;
    } else
    {
        cout << "create table fail ["
            << PQresultErrorMessage(result) << "]" << endl;
    }
    PQclear(result);

    result = PQexec(conn, CR_INDX_SQL.c_str());
    if(result && PQresultStatus(result) == PGRES_COMMAND_OK)
    {
        cout << "create index done" << endl;
    } else
    {
        cout << "create index fail ["
            << PQresultErrorMessage(result) << "]" << endl;
    }
    PQclear(result);

    PQfinish(conn);
}
