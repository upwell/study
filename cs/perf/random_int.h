#ifndef RANDOM_INT_H
#define RANDOM_INT_H

using namespace std;

class RandomInt
{
public:
    RandomInt(int cnt, int maxvalue);
    ~RandomInt() {}

    void Initialize();

private:
    int item_cnt_;
    int max_value_;
    set<int> items_;

};

#endif
