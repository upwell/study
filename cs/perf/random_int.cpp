#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <set>

#include "random_int.h"

RandomInt::RandomInt(int cnt, int maxvalue) :
    item_cnt_(cnt),
    max_value_(maxvalue)
{

}

void RandomInt::Initialize()
{
    int i = 0;
    while(items_.size() < item_cnt_)
    {
        srand(time(NULL) + i++);
        int item = rand() % max_value_;
        items_.insert(item);
    }
}

void usage(const char* proc_name)
{
    printf("Usage: %s <num_of_size>\n", proc_name);
    return;
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        usage(argv[0]);
        return -1;
    }

    int cnt = atoi(argv[1]);
    if(cnt <= 0)
    {
        usage(argv[0]);
        return -1;
    }

    RandomInt rint(cnt, RAND_MAX);
    rint.Initialize();

    return 0;
}
