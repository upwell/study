#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "identify.h"

pnode hash_array[HASH_SIZE];

void process_link(pnode* head, const char *word)
{
    pnode cur;
    int is_found = 0;

    if(NULL == *head)
    {
        pnode node = (pnode) malloc(sizeof(wnode)); 
        if(NULL == node)
            return;
        memset(node->word, '\0', WORD_LEN);
        strncpy(node->word, word, WORD_LEN);
        node->next = NULL;
        node->cnt = 1;
        *head = node;
        return;
    }

    cur = *head;
    while(NULL != cur)
    {
        if(strncmp(cur->word, word, WORD_LEN) == 0)
        {
            cur->cnt++;
            is_found = 1;
            break;
        }
        if(NULL == cur->next)
            break;
        cur = cur->next;
    }

    if(NULL == cur->next && !is_found)
    {
        pnode node = (pnode) malloc(sizeof(wnode));
        if(NULL == node)
            return;
        memset(node->word, '\0', WORD_LEN);
        strncpy(node->word, word, WORD_LEN);
        node->cnt = 1;
        node->next = NULL;
        cur->next = node;
    }

    return;
}

void process_word(char *word)
{
    int hval = 0;
    pnode *head = NULL;

    strtolower(word);

    head = &hash_array[new_hash(word)];
    process_link(head, word);

    return;
}

void process_line(const char *line)
{
    char word[WORD_LEN];
    int iw = 0;

    memset(word, '\0', WORD_LEN);

    while(*line != '\0')
    {
       if(ischar(*line))
       {
           word[iw] = *line;
           iw++;
       } else
       {
           if(strlen(word) > 0)
           {
               iw = 0;
               process_word(word);
               memset(word, '\0', WORD_LEN);
           }
       }
       line++;
    }

    return;
}

int small(int *arr, int len)
{
    int smallest = arr[0];
    int cur = 0;
    int i;

    for(i = 1; i < len; i++)
    {
       if(arr[i] < smallest)
       {
           smallest = arr[i];
           cur = i;
       }
    }

    return cur;
}

void gettop10()
{
    int num = 0;
    int smallest = 0;
    pnode top10[10];
    int ntop10[10];
    pnode p = NULL;
    int scur;
    int i;
    for(i = 0; i < 10; i++)
    {
        top10[i] = NULL;
        ntop10[i] = -1;
    }

    for(i = 0; i < HASH_SIZE; i++)
    {
        p = hash_array[i];
        while(NULL != p)
        {

            if(num < 10)
            {
                top10[num] = p;
                ntop10[num] = p->cnt;
                if(smallest == 0 || p->cnt < smallest)
                {
                    smallest = p->cnt;
                    scur = num;
                }
                num++;
            } else if(p->cnt > smallest)
            {
                top10[scur] = p;
                ntop10[scur] = p->cnt;
                scur = small(ntop10, 10);
                smallest = top10[scur]->cnt;
            }
            p = p->next;
        }
    }

    for(i = 0; i < 10; i++)
    {
        pnode tmp = top10[i];
        if(tmp != NULL)
            printf("[%s]\t\t[%d]\n", tmp->word, tmp->cnt);
    }
}

void print_usage()
{
    printf("./a.out <filename>\n");
}

int main(int argc, char *argv[])
{
    char *filename = NULL;
    char buf[LINE_SIZE];
    char *res = NULL;
    int i;

    if(argc < 2)
    {
        print_usage();
        return -1;
    }

    filename = argv[1];

    FILE *fp = fopen(filename, "r+");
    if(NULL == fp)
    {
        printf("file open failed\n");
        return -1;
    }

    for(i = 0; i < HASH_SIZE; i++)
        hash_array[i] = NULL;

    memset(buf, '\0', LINE_SIZE);
    while((res = fgets(buf, LINE_SIZE, fp)) != NULL)
    {
        //printf("%s", buf);
        process_line(buf);
    }

    gettop10();
    hash_stat(hash_array, HASH_SIZE);

    return 0;
}
