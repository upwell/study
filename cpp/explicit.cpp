#include <iostream>

class Foo
{
public:
    Foo() {}

    explicit Foo(int x)
    {
        val = x;
    }

    ~Foo() {}

private:
    int val;
};

int main()
{
    Foo f1(10);
    // without explicit, the following line
    // will be compiled without error
    Foo f2 = 10;

    std::cout << "hello" << std::endl;

    return 0;
}

