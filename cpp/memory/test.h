#include <iostream>

#ifndef _TEST_H
#define _TEST_H

using namespace std;

class test {
public:
    test(string name);
    ~test();

    const string getName() const;

private:
    string name_;
};

#endif
