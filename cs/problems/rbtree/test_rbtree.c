#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "ngx_rbtree.h"

#define TREE_SIZE 100
#define INT_MAX (TREE_SIZE*10)

typedef struct test_node_s
{
    ngx_rbtree_node_t node;
    unsigned int value;
} test_node_t;

int get_rand()
{
    return (rand() % INT_MAX);
}

void insert_func(ngx_rbtree_node_t *temp, ngx_rbtree_node_t *node,
        ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t **p;

    for( ;; )
    {
        if(node->key < temp->key)
            p = &temp->left;
        else if(node->key > temp->key)
            p = &temp->right;
        else {
            /* assign key as the same valude of ->value */
            p = &temp->left;
        }

        if(*p == sentinel)
            break;

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = node->right = sentinel;
    ngx_rbt_red(node);
}

void tree_walk(ngx_rbtree_node_t *root, ngx_rbtree_node_t *sentinel)
{
   if(root == sentinel)
       return;

   tree_walk(root->left, sentinel);
   test_node_t *tnode;
   {
       tnode = (test_node_t*) root;
       printf("%d ", tnode->value);
   }
   tree_walk(root->right, sentinel);
}

int main()
{
    ngx_rbtree_t trb;
    ngx_rbtree_node_t sentinel;
    int i;
    test_node_t *nodes;
    test_node_t *tnode;

    ngx_rbtree_init(&trb, &sentinel, insert_func);

    nodes = (test_node_t*) calloc(TREE_SIZE, sizeof(test_node_t));
    if(nodes == NULL)
    {
        perror("calloc nodes failed");
        return -1;
    }

    srand(time(NULL));
    for(i = 0; i < TREE_SIZE; i++)
    {
        tnode = &nodes[i];
        tnode->value = get_rand();
        tnode->node.key = tnode->value;

        ngx_rbtree_insert(&trb, &tnode->node);
    }

    tnode = (test_node_t*) ngx_rbtree_min(trb.root, trb.sentinel);

    printf("min value [%d]\n", tnode->value);

    tree_walk(trb.root, trb.sentinel);

    free(nodes);

    return 0;
}
