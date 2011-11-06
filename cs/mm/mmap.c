#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define ERROR -1

int main(int argc, char *argv[])
{
    void *ptr = NULL;
    int result = 0;
    struct stat file_stat;
    int file_size = 0, left_size = 0, len = 0;
    int fd = 0;
    off_t page_size = sysconf(_SC_PAGE_SIZE);

    if(argc != 2)
    {
        printf("usage: %s filename\n", argv[0]);
        return ERROR;
    }

    fd = open(argv[1], O_RDWR);
    if(fd < 0)
    {
        printf("open file error [%s]\n", strerror(errno));
        return ERROR;
    }

    fprintf(stderr, "_SC_PAGE_SIZE is [%u]\n", (unsigned int) page_size);

    memset(&file_stat, 0, sizeof(file_stat));
    result = fstat(fd, &file_stat);
    if(result < 0)
    {
        printf("fstat file error [%s]\n", strerror(errno));
        return ERROR;
    }

    file_size = file_stat.st_size;
    left_size = file_size;

    while(left_size > 0)
    {
        len = (left_size < page_size) ? left_size : page_size;

        ptr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, file_size-left_size);
        if(!ptr)
        {
            printf("mmap error [%s]\n", strerror(errno));
            return ERROR;
        }
        fprintf(stderr, "0x%p\n", ptr);

        fwrite(ptr, 1, len, stdout);
        munmap(ptr, len);
        left_size -= len;
    }

    close(fd);

    return 0;
}
