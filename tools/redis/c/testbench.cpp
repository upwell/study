#include "filemapping.h"

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

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <count>\n", argv[0]);
        return -1;
    }

    int count = atoi(argv[1]);

    FileMapping fm;
    fm.Init();

    srand(time(NULL));

    string lpath_base("//nj-fs/home/iwss/");
    string rpath_base("//nas1/home/iwss/ob/");

    long long start = getCurrentTime();

    for(int i = 0; i < count; i++)
    {
        string tmpStr1 = fidToString(rand());
        string tmpStr2 = fidToString(rand());
        string lpath = lpath_base + tmpStr1 + "/" + tmpStr2;
        string rpath = rpath_base + tmpStr2 + "/" + tmpStr1;
        fm.AddFile(lpath, rpath);
    }

    long long end = getCurrentTime();

    printf("Average handle time (us) : %lld\n", (end-start)/count);

    return 0;
}
