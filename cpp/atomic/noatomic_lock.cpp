#include <pthread.h>
#include <errno.h>
#include <stdio.h>

#include <iostream>
#include <atomic>

// compile with g++ atomic.cpp -lpthread -std=c++11

using namespace std;


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *run(void *args)
{
    int *pi = static_cast<int*>(args);

    pthread_mutex_lock(&lock);
    (*pi)++;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("usage: [%s] <thread_num>\n", argv[0]);
        return -1;
    }

    int thread_cnt = atoi(argv[1]);
    int a = 0;

    pthread_attr_t pattr;
    pthread_attr_init(&pattr);

    pthread_t *tids = new pthread_t[thread_cnt];

    for(int i = 0; i < thread_cnt; i++)
    {
        int rt = pthread_create(&tids[i], &pattr,
                run, &a);

        if(rt < 0)
        {
            perror("create thread failed\n");
        }
    }

    for(int i = 0; i < thread_cnt; i++)
        pthread_join(tids[i], NULL);

    cout << a << endl;
    delete []tids;

    return 0;
}

