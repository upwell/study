#ifndef RANDOM_INT_H
#define RANDOM_INT_H

using namespace std;

class RandomInt
{
public:
    RandomInt(int cnt, int maxvalue);
    ~RandomInt() {}

    void Initialize();
    void AddNewItem(int item);
    int Size() { return size_; }

private:
    int item_cnt_;
    int max_value_;
    int size_;
    int *items_;

};

#endif
