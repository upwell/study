#include <iostream>
#include <memory>
#include <tr1/memory>

#include "test.h"

test *get_instance()
{
    test *t = new test();

    return t;
}

int main()
{
    //std::auto_ptr<test> t_inst(get_instance()); /* no memory leak, but has share problem */
    std::tr1::shared_ptr<test> t_inst(get_instance());
    //test *t_inst = get_instance(); /*  memory leak */

    return 0;
}
