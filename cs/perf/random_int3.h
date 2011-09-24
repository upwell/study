#ifndef RANDOM_INT_H
#define RANDOM_INT_H

using namespace std;

typedef struct ItemData
{
    int item;
    ItemData *next;
} ItemData;

class RandomInt
{
public:
    RandomInt(int cnt, int maxvalue);
    ~RandomInt() {}

    void Initialize();
    void AddNewItem(int item);
    int Size() { return size_; }
    void PrintData();

private:
    int item_cnt_;
    int max_value_;
    int size_;
    ItemData *head_;

    ItemData *array_;

};

#endif
