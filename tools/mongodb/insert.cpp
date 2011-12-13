#include <iostream>

#include <mongo/client/dbclient.h>

using namespace mongo;
using namespace std;

void run()
{
    DBClientConnection c;
    c.connect("localhost");
    cout << "connected" << endl;

    //insert
    BSONObj p = BSON( "name" << "ken" << "age" << 20 );

    c.insert("test.persons", p);

    //query
    cout << "count: " << c.count("test.persons") << endl;

    {
        auto_ptr<DBClientCursor> cursor =
            c.query("test.persons", {});
        while(cursor->more())
            cout << cursor->next().toString() << endl;
    }

    {
        auto_ptr<DBClientCursor> cursor =
            c.query("test.persons", QUERY("age" << 20));
        while(cursor->more())
            cout << cursor->next().toString() << endl;
    }
}

int main()
{
    try {
        run();
    } catch(DBException &e) {
        cout << "caught " << e.what() << endl;
    }

    return 0;
}
