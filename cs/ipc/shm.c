#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int mem_id = 0;
    char *mem_buf = NULL;

    mem_id = shmget(IPC_PRIVATE, 409600, IPC_CREAT|SHM_R|SHM_W);
    if(mem_id < 0)
    {
        perror("shmget fail");
        exit(-1);
    }
    printf("mem_id is [%d]\n", mem_id);

    mem_buf = shmat(mem_id, NULL, 0);
    if(mem_buf == NULL)
    {
        perror("shmat fail");
        exit(-1);
    }
    shmctl(mem_id, IPC_RMID, NULL);

    if(fork() == 0)
    {
        mem_buf[0] = 'a';
        exit(0);
    }

    wait(NULL);
    getchar();
    printf("%c\n", mem_buf[0]);

    return 0;
}
