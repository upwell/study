#include "filemapping.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("usage: %s <lpath> <rpath>\n", argv[0]);
        return -1;
    }

    FileMapping fm;

    fm.Init();

    string lpath(argv[1]);
    string rpath(argv[2]);

    fm.AddFile(lpath, rpath);

    return 0;
}
