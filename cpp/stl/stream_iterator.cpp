#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

using namespace std;

int main()
{
    vector<string> vec;

    copy(istream_iterator<string>(cin),
            istream_iterator<string>(),
            back_inserter(vec));

    sort(vec.begin(), vec.end());

    unique_copy(vec.begin(), vec.end(),
            ostream_iterator<string>(cout, "\n"));

    return 0;
}
