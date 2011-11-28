#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <string.h>

#define ARRAY_LEN 50000
#define MAX_VALUE 1000000

void print_arr(int *arr, int len)
{
    int i = 0;

    for(i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return;
}

int get_rand()
{
    return (rand() % MAX_VALUE);
}

void init_array(int *array, int len)
{
    int i = 0;

    for(i = 0; i < len; i++)
        array[i] = get_rand();

    return;
}

long long getutime()
{
    struct timeval now;

    gettimeofday(&now, NULL);

    return (now.tv_sec*1000*1000 + now.tv_usec);
}

void insert_sort1(int *array, int len)
{
    int i, j;
    int tmp;

    for(i = 0; i < len; i++)
    {
        tmp = array[i];
        for(j = i - 1; j >= 0; j--)
        {
            if(tmp < array[j])
            {
                array[j+1] = array[j];
                array[j] = tmp;
            }
            else
            {
                array[j+1] = tmp;
                break;
            }
        }
    }
    return;
}

void insert_sort2(int *array, int len)
{
    int i, j;
    int tmp;

    for(i = 0; i < len; i++)
    {
        tmp = array[i];
        for(j = i - 1; j >= 0; j--)
        {
            if(tmp < array[j])
            {
                array[j+1] = array[j];
                if(j == 0) // little improvement here
                    array[j] = tmp;
            }
            else
            {
                array[j+1] = tmp;
                break;
            }
        }
    }
    return;
}

void quick_sort(int *array, int len)
{

}

void sort_func(void(*func)(int*, int), int *array, int len)
{
    char **func_name;
    func_name = backtrace_symbols(&func, 1);

    long long start = getutime();
    func(array, len);

    printf("%s takes [%lld]us\n", (func_name ? func_name[0] : ""),
            (getutime() - start));

    return;
}

int main()
{
    int *array, *array1, *array2, *array3;

    array = (int*) malloc(ARRAY_LEN * sizeof(int));
    array1 = (int*) malloc(ARRAY_LEN * sizeof(int));
    array2 = (int*) malloc(ARRAY_LEN * sizeof(int));
    array3 = (int*) malloc(ARRAY_LEN * sizeof(int));
    if(!array || !array1 || !array2 || !array3)
    {
        fprintf(stderr, "fail to malloc\b");
        exit(-1);
    }

    srand(time(NULL));
    init_array(array, ARRAY_LEN);

    memcpy(array1, array, ARRAY_LEN*sizeof(int));
    memcpy(array2, array, ARRAY_LEN*sizeof(int));
    memcpy(array3, array, ARRAY_LEN*sizeof(int));

    //print_arr(array, ARRAY_LEN);
    sort_func(insert_sort1, array, ARRAY_LEN);
    sort_func(insert_sort2, array1, ARRAY_LEN);
    //print_arr(array, ARRAY_LEN);

    return 0;
}
