#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

int main(int argc, char *argv[])
{
    int fd = 0, zero = 0, i = 0;
    int *addr = NULL;
    int nloops = 0;
    pid_t cpid = 0;
    sem_t *mutex;
    const char *sem_name = "mysem";

    if(argc != 2)
    {
        printf("usage: %s <loop number>\n", argv[0]);
        return -1;
    }

    nloops = atoi(argv[1]);

    fd = open("foo.out", O_RDWR|O_CREAT, 0644);
    if(fd < 0)
    {
        perror("open file fail");
        return -1;
    }
    write(fd, &zero, sizeof(zero));

    addr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == NULL)
    {
        perror("mmap fail");
        return -1;
    }
    close(fd);

    setbuf(stdout, NULL);

    mutex = sem_open(sem_name, O_CREAT|O_EXCL, 0644, 1);
    if(mutex == NULL)
    {
        perror("sem_open fail");
        return -1;
    }
    sem_unlink(sem_name);

    if((cpid = fork()) < 0)
    { // error
        perror("fork fail");
        exit(-1);
    } else if(cpid == 0)
    { // child
        for(i = 0; i < nloops; i++)
        {
            sem_wait(mutex);
            /* printf("child [%d]\n", (*addr)++); */
            (*addr)++;
            sem_post(mutex);
        }

        exit(0);
    }

    for(i = 0; i < nloops; i++)
    {
        sem_wait(mutex);
        /* printf("parent [%d]\n", (*addr)++); */
        (*addr)++;
        sem_post(mutex);
    }

    wait(NULL);

    exit(0);
}
