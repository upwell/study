#include <iostream>

template <class T>
inline void PRINT_ELEMENT(const T &coll)
{
    typename T::const_iterator iter;

    for(iter = coll.begin(); iter != coll.end(); ++iter)
        std::cout << *iter << ' ';
    std::cout << std::endl;

    return;
}

