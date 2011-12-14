#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <mongo/client/dbclient.h>

using namespace mongo;
using namespace std;

const int MAX_ID = 10*1000*1000;
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

void run(int cnt)
{
    DBClientConnection c;
    c.connect("localhost");

    srand(time(NULL));

    long long start = getCurrentTime();
    int found = 0;
    //insert
    for(int i = 0; i < cnt; i++)
    {
        int id = get_rand_value();
        auto_ptr<DBClientCursor> cursor =
            c.query(USER_CATE_COLLECTION, QUERY("id" << id));
        if(cursor->itcount() > 0)
            ++found;
    }
    long long end = getCurrentTime();

    //count
    cout << "count: " << c.count(USER_CATE_COLLECTION) << endl;
    cout << "average time: " << (end-start)/cnt << endl;
    cout << "found [" << found << "] in [" << cnt << "]" << endl;
}

int main(int argc, char *argv[])
{
    int cnt = 0;

    if(argc != 2)
    {
        cout << "usage ./" << argv[0] << " <search_cnt>" << endl;
        return -1;
    } else
        cnt = atoi(argv[1]);

    try {
        run(cnt);
    } catch(DBException &e) {
        cout << "caught " << e.what() << endl;
    }

    return 0;
}
