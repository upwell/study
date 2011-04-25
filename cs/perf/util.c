#include <stdio.h>
#include <stdlib.h>

#include "identify.h"

int hash(const char* word)
{
    int cnt = 0;

    if(NULL == word)
        return -1;

    while(*word != '\0')
    {
        cnt += *word;
        word++;
    }

    return cnt % HASH_SIZE;
}

int new_hash(const char* word)
{
    unsigned int ch = 0;

    while(*word != 0)
    {
        ch = (ch<<5) + ch + *word++;
    }

    return ch % HASH_SIZE;
}

int ischar(char s)
{
    if((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z'))
        return 1;
    else
        return 0;
}

void strtolower(char *str)
{
    if(NULL == str)
        return;

    while(*str != '\0')
    {
        if(*str >= 'A' && *str <= 'Z')
            *str += 32;
        str++;
    }
    return;
}

int get_cnt_link_nodes(pnode head)
{
    int cnt = 0;

    if(NULL == head)
        return 0;

    while(NULL != head)
    {
        cnt++;
        head = head->next;
    }

    return cnt;
}

void hash_stat(pnode hash[], int len)
{
    int *num_stat = (int*) malloc(len*sizeof(int));
    int i = 0;
    int total = HASH_SIZE;
    int zero = 0;
    int good = 0;
    int bad2 = 0;
    int bad3 = 0;
    int bad4 = 0;
    int vbad = 0;

    printf("id\t\tcnt\n"); for(i = 0; i < len; i++)
    {
        num_stat[i] = get_cnt_link_nodes(hash[i]);
        switch(num_stat[i])
        {
        case 0:
            zero++;
            break;
        case 1:
            good++;
            break;
        case 2:
            bad2++;
            break;
        case 3:
            bad3++;
            break;
        case 4:
            bad4++;
            break;
        default:
            vbad++;
        }
    }

    printf("total\t\t[%d]\n", total);
    printf("zero\t\t[%d]\n", zero);
    printf("good\t\t[%d]\n", good);
    printf("bad2\t\t[%d]\n", bad2);
    printf("bad3\t\t[%d]\n", bad3);
    printf("bad4\t\t[%d]\n", bad4);
    printf("vbad\t\t[%d]\n", vbad);

    free(num_stat);
}
