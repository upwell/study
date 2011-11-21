#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>

int main(int argc, char *argv[])
{
    int fd = 0, zero = 0, i = 0;
    int *addr = NULL;
    int nloops = 0;
    pid_t cpid = 0;
    pthread_mutex_t init_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t *p_mutex;
    pthread_mutexattr_t mattr;

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
    write(fd, &init_mutex, sizeof(init_mutex));

    addr = mmap(NULL, sizeof(int) + sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == NULL)
    {
        perror("mmap fail");
        return -1;
    }
    close(fd);

    p_mutex = (pthread_mutex_t*) (addr + 1);  // allocate pthread_mutex_t in the shared memory
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(p_mutex, &mattr);

    setbuf(stdout, NULL);

    if((cpid = fork()) < 0)
    { // error
        perror("fork fail");
        exit(-1);
    } else if(cpid == 0)
    { // child
        for(i = 0; i < nloops; i++)
        {
            pthread_mutex_lock(p_mutex);
            printf("child [%d]\n", (*addr)++);
            pthread_mutex_unlock(p_mutex);
        }

        exit(0);
    }

    for(i = 0; i < nloops; i++)
    {
        pthread_mutex_lock(p_mutex);
        printf("parent [%d]\n", (*addr)++);
        pthread_mutex_unlock(p_mutex);
    }

    wait(NULL);

    exit(0);
}
