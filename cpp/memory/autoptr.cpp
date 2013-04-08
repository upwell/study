#include <iostream>
#include <memory>
#include <tr1/memory>

#include "test.h"

test *get_instance(const string &name)
{
    test *t = new test(name);

    return t;
}

int main()
{
    std::auto_ptr<test> a_inst(get_instance("auto_ptr")); /* no memory leak, but has share problem */
    std::auto_ptr<test> a_inst_new(a_inst); // Now a_inst is null

    if(a_inst.get() == NULL)
    {
        cout << "a_inst is NULL" << endl;
    } else
    {
        cout << a_inst_new->getName() << endl;
    }

    std::tr1::shared_ptr<test> t_inst(get_instance("shared_ptr"));
    std::tr1::shared_ptr<test> t_inst_new(t_inst);

    cout << t_inst->getName() << endl;
    cout << t_inst_new->getName() << endl;
    //test *t_inst = get_instance(); /*  memory leak */

    return 0;
}

