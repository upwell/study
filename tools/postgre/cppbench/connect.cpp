#include <stdio.h>
#include <string>
#include <iostream>

#include "libpq-fe.h"

using namespace std;

int main()
{
    string conninfo = "host='localhost' port=5432 dbname='test' user='postgres'";

    PGconn *conn;

    conn = PQconnectdb(conninfo.c_str());
    if(conn && PQstatus(conn) != CONNECTION_BAD)
    {
        cout << "connected" << endl;
    }

    PQfinish(conn);
}
