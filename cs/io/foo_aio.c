#include <aio.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFSIZE 1024

int main()
{
    int fd, ret;
    struct aiocb my_aiocb;

    fd = open("file.txt", O_RDONLY);
    if(fd < 0) perror("open");

    bzero((char*)&my_aiocb, sizeof(struct aiocb));
    my_aiocb.aio_buf = malloc(BUFSIZE+1);
    if(!my_aiocb.aio_buf) perror("malloc");

    my_aiocb.aio_fildes = fd;
    my_aiocb.aio_nbytes = BUFSIZE;
    my_aiocb.aio_offset = 0;

    ret = aio_read(&my_aiocb);
    if(ret < 0) perror("aio_read");

    while(aio_error(&my_aiocb) == EINPROGRESS);

    if((ret = aio_return(&my_aiocb)) > 0)
    {
        printf("%d\n", ret);
    } else
    {
        perror("aio_return");
    }

    return 0;
}
