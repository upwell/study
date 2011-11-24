#include "filemapping.h"
#include "searchresult.h"

#include <iostream>

using namespace std;

int main()
{
    FileMapping fm;
    SearchResult result;

    fm.Init();

    fm.GetRPath("//nj-fs/home/iwss/han", result);

    cout << result.IsFound() << endl;

    if(result.IsFound())
        cout << result.GetExactRPath() << endl;

    return 0;
}
