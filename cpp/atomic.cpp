#include <pthread.h>
#include <errno.h>
#include <stdio.h>

#include <iostream>
#include <atomic>

// compile with g++ atomic.cpp -lpthread -std=c++11

using namespace std;

void *run(void *args)
{
    atomic_int *pi = static_cast<atomic_int*>(args);

    (*pi)++;

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
    atomic_int a;
    a.store(0);

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

    cout << a.load() << endl;

    delete []tids;

    return 0;
}
