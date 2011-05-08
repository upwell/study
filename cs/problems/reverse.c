#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

#define LENGTH 16

void print_array(int arr[], int size)
{
    if(!arr)
    {
        printf("empty array\n");
        return;
    }

    int i = 0;
    printf("Array Content:\n");
    for(; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return;
}

void reverse_array(int arr[], int size)
{
    if(!arr)
    {
        printf("empty array\n");
        return;
    }

    printf("reverse array ...\n");

    int i = 0, tmp = 0;
    int half = size/2;

    for(; i < half; i++)
    {
        tmp = arr[i];
        arr[i] = arr[size-i-1];
        arr[size-i-1] = tmp;
    }

    return;
}

int main()
{
    int array[SIZE];
    int i = 0;
    char tmp_num[LENGTH];

    printf("Enter %d integers :\n", SIZE);
    while(i < SIZE)
    {
        memset(tmp_num, '\0', LENGTH);
        scanf("%s", tmp_num);
        array[i] = atoi(tmp_num);
        i++;
    }
    print_array(array, SIZE);
    reverse_array(array, SIZE);
    print_array(array, SIZE);

    return 0;
}
