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
const string TABLE_NAME = "usergroup5";

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

void run(int batch, int cnt)
{
    string conninfo = "host='localhost' port=5432 dbname='test' user='postgres'";

    PGconn *conn;
    conn = PQconnectdb(conninfo.c_str());
    if(conn && PQstatus(conn) != CONNECTION_BAD)
    {
        cout << "connected" << endl;
    } else
    {
        cout << "error happened" << endl;
        PQfinish(conn);
        return;
    }

    srand(time(NULL));

    long long start = getCurrentTime();
    //insert
    for(int i = 0; i < cnt; )
    {
        int rep = (batch < cnt-i) ? batch : cnt-i;
        string batchCmd;

        int total = 99;

        for(int j = 0; j < rep; j++)
        {
            string cmd = "insert into " + TABLE_NAME + " ( value ) values ('";

            for(int k = 1; k <= total; k++)
            {
                char tmp[256];
                memset(tmp, 0, sizeof(tmp));

                snprintf(tmp, sizeof(tmp) - 1, "%d:%d;", k, k);

                cmd += tmp;
            }

            cmd += "');";

            batchCmd += cmd;
        }

        PGresult *result;
        result = PQexec(conn, batchCmd.c_str());
        if(result && PQresultStatus(result) ==
                PGRES_COMMAND_OK)
            ;
        else
            cout << "insert failed ["
                << PQresultErrorMessage(result) << "]" << endl;
        PQclear(result);

        i += rep;
    }

    long long end = getCurrentTime();

    //count
    cout << "count: " << cnt << endl;
    cout << "average time: " << (end-start)/cnt << endl;

    return;
}

int main(int argc, char *argv[])
{
    int cnt = 0;
    int batch = 0;

    if(argc != 3)
    {
        cout << "usage " << argv[0] << " <batch_cnt> <insert_cnt>" << endl;
        return -1;
    } else
    {
        batch = atoi(argv[1]);
        cnt = atoi(argv[2]);
    }

    run(batch, cnt);

    return 0;
}
