#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

typedef struct {
    int read_pipe;
    int write_pipe;
} data_t;

/*
 * threads share file descriptors across the processes...
 * threads share the global data, global stack and heap, signal handling,
 * each thread has its own stack and error no, thread related data
 *
 */

void *emit(void *data)
{
    data_t *pip = (data_t*) data;
    int fd = open("text.txt", O_CREAT|O_RDWR, 0644);

    if(fd == -1)
    {
        perror("open file text.txt fail");
        pthread_exit(NULL);
    }

    write(fd, "hello", 5);

    printf("in thread emit fd [%d]\n", fd);
    write(pip->write_pipe, &fd, sizeof(fd));
    // close(fd); /* with this, the result is unexpected; */

    return NULL;
}

void *receive(void *data)
{
    data_t *pip = (data_t*) data;
    int fd;

    if(read(pip->read_pipe, &fd, sizeof(fd)) < 0)
    {
        perror("receive read pipe fail \n");
        pthread_exit(NULL);
    }

    if(write(fd, "receive", 7) < 0)
    {
        perror("receive write file fail \n");
    }

    printf("in thread receive fd [%d] \n", fd);

    close(fd);

    pthread_exit(NULL);
}

int main()
{
    pthread_t emitter, receiver;
    int pipefd[2];
    int res;
    data_t data;

    if(pipe(pipefd) == -1)
    {
        perror("call pipe fail");
        return -1;
    }

    data.read_pipe = pipefd[0];
    data.write_pipe = pipefd[1];

    if(pthread_create(&emitter, NULL, emit, &data) != 0)
    {
        perror("create thread failed");
        return -1;
    }

    if(pthread_create(&receiver, NULL, receive, &data) != 0)
    {
        perror("create thread failed");
        return -1;
    }

    pthread_join(emitter, NULL);
    pthread_join(receiver, NULL);

}
