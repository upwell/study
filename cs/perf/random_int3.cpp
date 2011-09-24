#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include <set>

#include "random_int3.h"

RandomInt::RandomInt(int cnt, int maxvalue) :
    item_cnt_(cnt),
    max_value_(maxvalue),
    size_(0),
    head_(NULL),
    array_(NULL)
{

}

void RandomInt::Initialize()
{
    int i = 0;

    array_ = (ItemData*) calloc(item_cnt_, sizeof(ItemData));
    if(NULL == array_)
    {
        printf("failed to calloc memory\n");
        exit(-1);
    }

    while(size_ < item_cnt_)
    {
        srand(time(NULL) + i++);
        int item = rand() % max_value_;
        AddNewItem(item);
    }
}

void RandomInt::PrintData()
{
   ItemData *start = head_; 

   while(start)
   {
       printf("%d\n", start->item);
       start = start->next;
   }

   printf("\n");
   return;
}

void RandomInt::AddNewItem(int item)
{
    ItemData *tmp = NULL;

    if(NULL == head_)
    {
        tmp = &array_[size_];
        tmp->item = item;
        tmp->next = NULL;
        head_ = tmp;
        size_++;
        return;
    }

    if(item < head_->item)
    {
        tmp = &array_[size_];
        tmp->item = item;
        tmp->next = head_;
        head_ = tmp;
        size_++;
        return;
    }

    ItemData *start = head_;
    ItemData *prev = head_;

    while(NULL != start)
    {
        if(item == start->item)
            return;
        else if(item < start->item)
            break;
        else
        {
            prev = start;
            start = start->next;
        }
    }

    tmp = &array_[size_];
    tmp->item = item;
    tmp->next = prev->next;
    prev->next = tmp;
    size_++;

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
