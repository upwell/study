#include <vector>
#include <algorithm>
#include <iostream>

int main()
{
    typedef std::vector<int> INTVECT;

    INTVECT tvect;
    INTVECT::iterator pos;

    for(int i = 0; i < 10; i++)
        tvect.push_back(std::rand() % 100);

    for(pos = tvect.begin(); pos != tvect.end(); ++pos)
        std::cout << *pos << ' ';
    std::cout << std::endl;

    pos = max_element(tvect.begin(), tvect.end());
    std::cout << "max: " << *pos << std::endl;

    pos = min_element(tvect.begin(), tvect.end());
    std::cout << "min: " << *pos << std::endl;

    sort(tvect.begin(), tvect.end());
    for(pos = tvect.begin(); pos != tvect.end(); ++pos)
        std::cout << *pos << ' ';
    std::cout << std::endl;

    reverse(tvect.begin(), tvect.end());
    for(pos = tvect.begin(); pos != tvect.end(); ++pos)
        std::cout << *pos << ' ';
    std::cout << std::endl;

    return 0;
}
