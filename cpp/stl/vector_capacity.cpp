#include <iostream>
#include <vector>
#include <cstdlib>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "usage: " << argv[0] <<
            " <element_number> " << std::endl;
        return 1;
    }

    int number = atoi(argv[1]);

    std::vector<int> foo;
    foo.reserve(number);

    for(int i = 0; i < number; ++i)
    {
        foo.push_back(i);
    }

    return 0;
}
