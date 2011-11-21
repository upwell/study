#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <queue>

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_sig = PTHREAD_COND_INITIALIZER;


const int total_producer = 2;
const int total_consumer = 5;
bool is_done = false;

queue<int> data;

void *run(void *args)
{
    int tnum = reinterpret_cast<intptr_t>(args);

    pthread_mutex_lock(&mutex);
    // if this area is not locked, the output will mess up
    cout << "work thread ok - [" << tnum << "]" << endl;
    pthread_mutex_unlock(&mutex);

    for(int i = 0; i < 10000000; i++)
    {
        pthread_mutex_lock(&data_mutex);
        data.push(i);
        if(data.size() == 1)
            pthread_cond_signal(&cond_sig);
        pthread_mutex_unlock(&data_mutex);
    }

    return NULL;
}

void *consume(void *args)
{
    int tnum = reinterpret_cast<intptr_t> (args);
    pthread_mutex_lock(&mutex);
    cout << "consume thread ok - [" << tnum << "]" << endl;
    pthread_mutex_unlock(&mutex);

    while(true)
    {
        pthread_mutex_lock(&data_mutex);
        if(0 == data.size())
        {
            if(is_done)
            {
                // nerver forget unlock here 
                pthread_mutex_unlock(&data_mutex);
                pthread_exit(NULL);
            }
            cerr << "queue is empty, wait ..." << endl;
            pthread_cond_wait(&cond_sig, &data_mutex);
        }
        if(data.size() > 0)
        {
            data.pop();
        }
        pthread_mutex_unlock(&data_mutex);
    }

    return NULL;
}

int main()
{
    pthread_t tid[total_producer];
    pthread_t cid[total_consumer];

    for(int i = 0; i < total_producer; i++)
        pthread_create(&tid[i], NULL, run, (void*)i);
    for(int i = 0; i < total_consumer; i++)
        pthread_create(&cid[i], NULL, consume, (void*)i);

    for(int i = 0; i < total_producer; i++)
        pthread_join(tid[i], NULL);
    cout << "all work threads exit" << endl;
    is_done = true;
    // notify the pending threads to exit
    pthread_cond_broadcast(&cond_sig);

    for(int i = 0; i < total_consumer; i++)
        pthread_join(cid[i], NULL);
    cout << "all consume threads exit" << endl;

    return 0;
}
