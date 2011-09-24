#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include <set>

#include "random_int1.h"

RandomInt::RandomInt(int cnt, int maxvalue) :
    item_cnt_(cnt),
    max_value_(maxvalue),
    size_(0)
{

}

void RandomInt::Initialize()
{
    items_ = (int*) calloc(item_cnt_, sizeof(int));
    if(NULL == items_)
    {
        printf("malloc failed [%s]", strerror(errno));
        exit(-1);
    }

    int i = 0;
    while(Size() < item_cnt_)
    {
        srand(time(NULL) + i++);
        int item = rand() % max_value_;
        AddNewItem(item);
    }
}

void RandomInt::AddNewItem(int item)
{
    int i = 0, j = 0;
    for(; items_[i] > item; i++)
        ;

    if(items_[i] == item)
        return;

    size_++;
    for(j = size_; j > i; j--)
    {
        items_[j] = items_[j-1];
    }
    items_[i] = item;

    return;
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
    printf("%d\n", rint.Size());

    return 0;
}
