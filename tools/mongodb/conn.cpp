#include <iostream>

#include <mongo/client/dbclient.h>

using namespace mongo;
using namespace std;

void run()
{
    DBClientConnection c;
    c.connect("localhost");
}

int main()
{
    try {
        run();
        cout << "connected" << endl;
    } catch(DBException &e) {
        cout << "caught " << e.what() << endl;
    }

    return 0;
}
