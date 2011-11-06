#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int fd = 0;
    int rtn = 0;

    fd = open("test.file", O_CREAT|O_RDWR);
    rtn = dup2(fd, 1); /* make stdin fd points to "test.file" */
    if(rtn < 0)
    {
        perror("dup2 failed");
        exit(-1);
    }

    /* this step actually print the "string" to stdin fd */
    printf("hello world in test.file\n"); 

    return 0;
}

