#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "eb32tree.h"

#define TREE_SIZE 10000000
#define INT_MAX (TREE_SIZE*10)

int get_rand()
{
    return (rand() % INT_MAX);
}

int main()
{
    int i;

    struct eb_root e32 = EB_ROOT;
    struct eb32_node *nodes;
    struct eb32_node *tnode;

    nodes = calloc(TREE_SIZE, sizeof(*nodes));
    if(nodes == NULL)
    {
        perror("calloc failed\n");
        return -1;
    }

    srand(time(NULL));

    for(i = 0; i < TREE_SIZE; i++)
    {
        tnode = nodes + i;
        tnode->key = get_rand();
        eb32_insert(&e32, tnode);
    }

    tnode = eb32_first(&e32);

    if(tnode)
        printf("min value [%d]\n", tnode->key);

    return 0;
}
