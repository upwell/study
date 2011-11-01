#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * create a file "file.foo" with some contents
 * create a hard link with #ln file.foo file.foo.hard
 *
 * after file.foo is unlinked, the data in file.foo.hard is
 * appended with "after unlink"
 *
 */

int main()
{
    int fd = 0, res = 0;
    char buf[8];
    char *data_tw = "after unlink";

    fd = open("file.foo", O_RDWR);
    memset(buf, 0, sizeof(buf));
    res = read(fd, buf, sizeof(buf) -1);
    if(res < 0) {
        perror("read error");
        return -1;
    }
    printf("read [%s]\n", buf);

    struct stat file_stat;
    fstat(fd, &file_stat);

    if(unlink("file.foo") < 0)
    {
        perror("unlink error");
        return -1;
    }

    if(lseek(fd, 0, SEEK_END) < 0)
    {
        perror("lseek error");
        return -1;
    }
    if(write(fd, data_tw, strlen(data_tw)) < 0)
    {
        perror("write error");
        return -1;
    }

    close(fd);

    return 0;
}
