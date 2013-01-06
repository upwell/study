#include <iostream>
#include <set>
#include <string>

using namespace std;

class CustomCmp
{
public:
    CustomCmp() {}
    ~CustomCmp() {}

    bool operator() (const string& s1, const string& s2) const
    {
        return s1.size() < s2.size();
    }
};

int main(int argc, const char *argv[])
{
    multiset<string, CustomCmp> st;

    st.insert("abcd");
    st.insert("def");
    st.insert("dafgh");
    st.insert("daewa");

    set<string>::iterator itr = st.begin();

    for(; itr != st.end(); itr++)
        cout << *itr << endl;

    return 0;
}

