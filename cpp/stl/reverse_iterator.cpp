#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

int main()
{
    vector<int> vec;

    for(int i = 0; i < 10; ++i)
        vec.push_back(i);

    copy(vec.rbegin(), vec.rend(),
            ostream_iterator<int>(cout, " "));

    return 0;
}
