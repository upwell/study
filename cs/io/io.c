#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>

int main()
{
    int fd1, fd2;
    char c;

    fd1 = open("test.file", O_RDONLY, 0);

    if(fork() == 0)
    {
        read(fd1, &c, 1);
        exit(0);
    }

    wait(NULL);
    read(fd1, &c, 1);
    printf("c=%c\n", c);

    return 0;
}
