#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include <set>

#include "random_int4.h"

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

ItemData* RandomInt::ReverseInsert(ItemData *head, int item)
{
    if(NULL == head)
    {
        head = &array_[size_];
        head->left = head->right = NULL;
        head->item = item;
        size_++;
    } else if(item < head->item)
    {
        head->left = ReverseInsert(head->left, item);
    } else if(item > head->item)
    {
        head->right = ReverseInsert(head->right, item);
    }

    return head;
}

void RandomInt::AddNewItem(int item)
{
    head_ = ReverseInsert(head_, item);

    return;
}

void RandomInt::Traverse(ItemData* head)
{
    if(head == NULL)
        return;

    Traverse(head->left);
    printf("%d\n", head->item);
    Traverse(head->right);

    return;
}

void RandomInt::PrintData()
{
    Traverse(head_);
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
//    rint.PrintData();

    return 0;
}
