#include "filemapping.h"
#include "searchresult.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string fidToString(const int fid)
{
    char buf[128];

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d", fid);

    string tmpStr(buf);

    return tmpStr;
}

// return the ms since 1970
long long getCurrentTime()
{
    struct timeval now;
    gettimeofday(&now, NULL);

    return (now.tv_sec*1000*1000 + now.tv_usec);
}

int getRand(int max)
{
    return (rand() % max);
}

const int MAX = 1000000;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <count>\n", argv[0]);
        return -1;
    }

    int count = atoi(argv[1]);

    int exact_count = 0;
    int found_count = 0;

    FileMapping fm;
    fm.Init();

    srand(time(NULL));

    string lpath_base("//nj-fs/home/iwss/");
    string rpath_base("//nas1/home/iwss/ob/");

    long long start = getCurrentTime();

    for(int i = 0; i < count; i++)
    {
        SearchResult result;

        string tmpStr1 = fidToString(getRand(MAX));
        string tmpStr2 = fidToString(getRand(MAX));

        string lpath = lpath_base + tmpStr1 + "/" + tmpStr2;

        fm.GetRPath(lpath, result);
        if(result.IsExactFound())
            exact_count++;
        if(result.IsFound())
            found_count++;
    }

    long long end = getCurrentTime();

    printf("Average handle time (us) : %lld\n", (end-start)/count);
    printf("Exact found [%d] out of [%d]\n", exact_count, count);
    printf("Found [%d] out of [%d]\n", found_count, count);

    return 0;
}
