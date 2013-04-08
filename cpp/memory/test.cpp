#include <iostream>

#include "test.h"

test::test(string name) : name_(name)
{
    cout << name_ + " : inside constructer" << endl;
}

test::~test()
{
    cout << name_ + " : insider destructer" << endl;
}

const string test::getName() const
{
    return name_;
}
