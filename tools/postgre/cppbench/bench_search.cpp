#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#include "libpq-fe.h"
using namespace std;

const int MAX_ID = 10*1000*1000;
const int MAX_CMD_LEN = 1024;
const string TABLE_NAME = "usergroup";

long long getCurrentTime()
{
    struct timeval now;
    gettimeofday(&now, NULL);

    return (now.tv_sec*1000*1000 + now.tv_usec);
}

int get_rand_value()
{
    return rand() % MAX_ID;
}

bool is_id_exist(PGconn *c, int id)
{
    char cmd[MAX_CMD_LEN];
    bool rtn = false;
    memset(cmd, 0, sizeof(cmd));

    snprintf(cmd, sizeof(cmd)-1, "select * from %s "
            "where id = '%d'", TABLE_NAME.c_str(), id);

    PGresult *result;
    result = PQexec(c, cmd);
    if(result && PQresultStatus(result) ==
            PGRES_TUPLES_OK)
    {
        if(PQntuples(result) > 0)
            rtn = true;
    } else
        cout << " select fail [" <<
            PQresultErrorMessage(result) << "]" << endl;

    PQclear(result);

    return rtn;
}

void run(const string &host, int cnt)
{
    string conninfo;
    if(!host.compare("-d"))
        conninfo = "host='/tmp' port='5432' dbname='test'";
    else
        conninfo = "host='" + host 
            + "' port=5432 dbname='test'";

    PGconn *conn;
    conn = PQconnectdb(conninfo.c_str());
    if(conn && PQstatus(conn) != CONNECTION_BAD)
    {
        cout << "connected" << endl;
    } else
    {
        cout << "error happened : " << 
            PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return;
    }

    srand(time(NULL));

    long long start = getCurrentTime();
    int found = 0;
    //insert
    for(int i = 0; i < cnt; i++)
    {
        int id = get_rand_value();
        char cmd[MAX_CMD_LEN];
        memset(cmd, 0, sizeof(cmd));

        snprintf(cmd, sizeof(cmd) - 1, "select * from %s where id = "
                "'%d'", TABLE_NAME.c_str(), id);
        PGresult *result;
        result = PQexec(conn, cmd);
        if(result && PQresultStatus(result) ==
                PGRES_TUPLES_OK)
        {
            if(PQntuples(result) > 0)
                found++;
        }
        else
            cout << "select failed ["
                << PQresultErrorMessage(result) << "]" << endl;
        PQclear(result);
    }
    long long end = getCurrentTime();

    //count
    cout << "count: " << cnt << endl;
    cout << "average time: " << (end-start)/cnt << endl;
    cout << "found [" << found << "] in [" << cnt << "]" << endl;

    return;
}

int main(int argc, char *argv[])
{
    int cnt = 0;
    string host;

    if(argc != 3)
    {
        cout << "usage " << argv[0] << " <host> <search_cnt>" << endl;
        return -1;
    } else
    {
        host = argv[1];
        cnt = atoi(argv[2]);
    }

    run(host, cnt);

    return 0;
}
