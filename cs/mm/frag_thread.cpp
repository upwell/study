#include <iostream>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

/*
 * This program gives an example of memory fragmentation,
 * allocate both LARGE_BULK and SMALL_BULK memory,
 * then release SMALL_BULK, the SMALL_BULK is not available.
 *
 * even use Google's tcmalloc this situation is not improved.
 */

const int TOTAL_MEM = 800 << 20; // 800 MB
const int LARGE_BULK = 1 << 20;  // 1MB
const int SMALL_BUIK = 300 << 10;// 300KB

const int vec_size = TOTAL_MEM/LARGE_BULK;

long long allocated = 0, filled = 0;

int pipe_t[2];
vector<char*> large_vec(vec_size);
vector<char*> small_vec(vec_size*2);

bool is_frag = false;

void* frag(void *args)
{
    for(int i = 0; i < vec_size; i++)
    {
        large_vec[i] = new char[LARGE_BULK];
        small_vec[i] = new char[SMALL_BUIK];
        small_vec[i+vec_size] = new char[SMALL_BUIK];
        allocated += LARGE_BULK + 2*SMALL_BUIK;
    }

    write(pipe_t[1], "G", 1);

    pthread_exit(NULL);
}

void* gen(void *args)
{
    char buff[12];
    read(pipe_t[0], buff, 1);

    if(is_frag)
    {
        for(int i = 0; i < vec_size*2; i++)
        {
            delete []small_vec[i];
            allocated -= SMALL_BUIK;
            filled += SMALL_BUIK;
        }
    }

    while(NULL != malloc(LARGE_BULK))
    {
        allocated += LARGE_BULK;
    }

    perror("malloc error");

    printf("allocated [%lld] MB\n", allocated/(1024*1024)); 
    printf("filled [%lld] MB\n", filled/(1024*1024)); 

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    if(argc == 1)
        is_frag = false;
    else if(argc == 2 && !strcmp(argv[1], "f"))
        is_frag = true;
    else
    {
        cout << "usage: " << argv[0] <<
            " or " << argv[0] << " f" << endl;
        return -1;
    }

    pthread_t frag_t, gen_t;

    pipe(pipe_t);

    pthread_create(&frag_t, NULL, frag, NULL);
    pthread_create(&gen_t, NULL, gen, NULL);

    pthread_join(frag_t, NULL);
    pthread_join(gen_t, NULL);

    return 0;
}
