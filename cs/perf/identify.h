#ifndef PERF_IDENTIFY_H_
#define PERF_IDENTIFY_H_

#define LINE_SIZE 1024
#define HASH_SIZE 2048
#define WORD_LEN 256

typedef struct word_node
{
    char word[WORD_LEN];
    int cnt;
    struct word_node *next;
} wnode, *pnode;

#endif
