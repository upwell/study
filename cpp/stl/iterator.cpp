#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

int main()
{
    vector<char> vt;

    for(char t = 'a'; t <= 'z'; ++t)
    {
        vt.push_back(t);
    }

    vector<char>::iterator it;
    /*
     * Generally it's better to use ++it than it++.
     * As it++ need to return the old position, which would
     * generate a temp variable, and it's slower
     */
    for(it = vt.begin(); it != vt.end(); ++it)
    {
        cout << *it << ' ';
    }

    cout << endl;

    return 0;
}

