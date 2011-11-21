#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>


int main(int argc, char *argv[])
{
    const char *filename = "test.out";
    const char *string = "hello world";
    int fd = open(filename, O_RDWR|O_CREAT, 0644);
    struct stat fs;
    void *ptr;
    int opt;
    int flag = 0;

    while((opt = getopt(argc, argv, "s")) != -1)
    {
        switch(opt) {
            case 's':
                flag = 1;
                break;
            default:
                break;
        }
    }

    if(fd < 0)
    {
        perror("open file fail");
        return -1;
    }

    write(fd, string, strlen(string));

    if(fstat(fd, &fs) < 0)
    {
        perror("fstat file fail");
        return -1;
    }

    if(flag)
        ptr = mmap(NULL, fs.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    else
        ptr = mmap(NULL, fs.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);

    if(ptr == NULL)
    {
        perror("mmap fail");
        return -1;
    }

    *((char *)ptr) = 'a';

    munmap(ptr, fs.st_size);

    close(fd);

    return 0;
}
