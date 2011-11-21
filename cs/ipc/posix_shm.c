#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main()
{
    const char *path = "/test.foo";
    int fd = 0;
    int length = 1024;
    void *addr;
    int i = 0;
    char *ptr;

    /* the path must be started with /, and with no / after */
    /* this will create a file named with path under /dev/shm/ */
    /* this seems to be platform dependent */
    fd = shm_open(path, O_CREAT|O_RDWR, S_IRWXU);
    if(fd < 0)
    {
        perror("shm_open call fail");
        exit(-1);
    } else {
        printf("shm created [%d]\n", fd);
    }

    if(ftruncate(fd, length) < 0)
    {
        perror("ftruncate fail");
        exit(-1);
    }

    addr = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(NULL == addr)
    {
        perror("mmap fail");
        exit(-1);
    }
    close(fd);

    ptr = (char*) addr;
    for(; i < length; i++)
    {
        *ptr++ = i%256;
    }

    return 0;
}
