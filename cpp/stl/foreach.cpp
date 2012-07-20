#include <vector>
#include <iostream>
#include <algorithm>

#include "print.h"

using namespace std;

class PrintIt
{
public:
    void operator()(int x)
    {
        cout << x << ' ';
        return;
    }
};

void print(int x)
{
    cout << x << ' ';

    return;
}

int main()
{
    vector<int> coll;

    for(int i = 1; i < 10; ++i)
        coll.push_back(i);

    for_each(coll.begin(), coll.end(),
            print);

    cout << endl;

    PRINT_ELEMENT(coll);

    PrintIt pi;
    for_each(coll.begin(), coll.end(),
            pi);

    return 0;
}
