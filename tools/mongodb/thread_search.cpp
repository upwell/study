#include <iostream>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <mongo/client/dbclient.h>

using namespace mongo;
using namespace std;

const int MAX_ID = 100*1000*1000;
const string USER_CATE_COLLECTION = "user.category";

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

bool is_id_exist(DBClientConnection &c, int id)
{
    if(c.count(USER_CATE_COLLECTION, BSON("id" << id)) > 0)
        return true;
    else
        return false;
}

void* run(void *args)
{
    int cnt = *(static_cast<int*>(args));
    int *found = new int;
    *found = 0;

    DBClientConnection c;
    c.connect("localhost");

    for(int i = 0; i < cnt; i++)
    {
        int id = get_rand_value();
auto_ptr<DBClientCursor> cursor =
            c.query(USER_CATE_COLLECTION, QUERY("id" << id));
        if(cursor->itcount() > 0)
            ++(*found);
    }

    pthread_exit(found);

    return NULL;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        cout << "usage: " << argv[0] << " <thread_num> <total_cnt>" << endl;
        return -1;
    }

    int thread_num = atoi(argv[1]);
    int cnt = atoi(argv[2]);

    pthread_t *tids;
    int result;

    tids = new pthread_t[thread_num];

    srand(time(NULL));

    long long start = getCurrentTime();

    for(int i = 0; i < thread_num; i++)
    {
        result = pthread_create(&tids[i], NULL, run, &cnt);
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
