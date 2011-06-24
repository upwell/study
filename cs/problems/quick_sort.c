/*
 * =====================================================================================
 *
 *       Filename:  quick_sort.c
 *
 *    Description:  Quick Sort
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Han He (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>

void print_arr(int *arr, int len)
{
    int i = 0;

    for(i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return;
}

void insert_sort(int *arr, int len)
{
    int i = 0, j = 0, k = 0;
    int tmp = 0;

    for(i = 0; i < len; i++)
    {
        tmp = arr[i];

        for(j = 0; j < i; j++)
            if(tmp <= arr[j])
                break;
        if(j == i)
            continue;

        for(k = i; k >= j; k--)
            arr[k] = arr[k-1];
        arr[j] = tmp;
    }

    return;
}

int main()
{
    int arr[] = { 9, 8, 6, 5, 7, 4};
    int len = sizeof(arr) / sizeof(arr[0]);

    print_arr(arr, len);
    insert_sort(arr, len);
    print_arr(arr, len);

    return 0;
}
