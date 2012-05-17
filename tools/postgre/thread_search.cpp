#include <iostream>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "libpq-fe.h"

using namespace std;

const int MAX_ID = 10*1000*1000;
const int MAX_CMD_LEN = 1024;
const string TABLE_NAME = "usergroup";

typedef struct thread_args 
{
    bool is_domain_sock;
    string host;
    int cnt;
} ThreadArgs;

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

void* run(void *args)
{
    ThreadArgs *targ = (static_cast<ThreadArgs*>(args));
    int *found = new int;
    *found = 0;

    int cnt = targ->cnt;

    string conninfo;
    if(!targ->is_domain_sock)
        conninfo = "host='" + targ->host +
           "' port=5432 dbname='test'";
    else
        conninfo = "dbname='test'";

    PGconn *conn;
    conn = PQconnectdb(conninfo.c_str());
    if(conn && PQstatus(conn) != CONNECTION_BAD)
    {
        cout << "connected" << endl;

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
                    (*found)++;
            }
            else
                cout << "select failed ["
                    << PQresultErrorMessage(result) << "]" << endl;
            PQclear(result);
        }
    }
    else
        cout << "error happened" << endl;


    PQfinish(conn);
    pthread_exit(found);

    return NULL;
}

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        cout << "./" << argv[0] << " <host_or_domain> <thread_num> <total_cnt>" << endl;
        return -1;
    }

    string domain = argv[1];
    int thread_num = atoi(argv[2]);
    int cnt = atoi(argv[3]);
    ThreadArgs args;
    args.cnt = cnt;
    if(!domain.compare("-d"))
        args.is_domain_sock = true;
    else
    {
        args.is_domain_sock = false;
        args.host = domain;
    }

    pthread_t *tids;
    int result;

    tids = new pthread_t[thread_num];

    srand(time(NULL));

    long long start = getCurrentTime();

    for(int i = 0; i < thread_num; i++)
    {
        result = pthread_create(&tids[i], NULL, run, &args);

        if(result != 0)
        {
            cout << "create thread [" << i << "] failed : ["
                << strerror(result) << "]" << endl;
            tids[i] = -1;
        }
    }

    int found = 0;
    for(int i = 0; i < thread_num; i++)
    {
        if(tids[i] != -1)
        {
            void *retval;
            pthread_join(tids[i], &retval);

            if(retval)
            {
                found += *(static_cast<int*>(retval));
                delete static_cast<int*>(retval);
            }
        }
    }

    long long end = getCurrentTime();

    int total_cnt = cnt*thread_num;
    cout << "count: " << total_cnt << endl;
    cout << "average time: " << (end-start)/total_cnt << endl;
    cout << "found [" << found << "] in [" << total_cnt << "]" << endl;

    delete []tids;

    return 0;
}
