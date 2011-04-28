#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node *next;
} Node;

void process_link(Node* link)
{
    while(link)
    {
        printf("%d ", link->val);
        link = link->next;
    }
    printf("\n");

    return;
}

Node *create_link(int *a, int len)
{
    int i = 0;
    Node *head = NULL;
    Node *last = NULL;
    Node *node = NULL;

    for(; i < len; i++)
    {
        node = (Node*) malloc(sizeof(Node));
        if(i == 0)
        {
            head = node;
            last = node;
        }
        node->val = a[i];
        node->next = NULL;

        last->next = node;
        last = node;
    }

    return head;
}

Node *merge_link(Node *na, Node *nb)
{
    if(!na || !nb) return NULL;

    Node *master, *slave, *pre, *result, *tmp;

    if(na->val > nb->val)
    {
        master = nb;
        slave = na;
    } else
    {
        master = na;
        slave = nb;
    }
    result = master;

    while(slave != NULL && master != NULL)
    {
        if(slave->val > master->val)
        {
            pre = master;
            master = master->next;
        } else
        {
            tmp = slave->next;
            pre->next = slave;
            slave->next = master;
            slave = tmp;
        }
    }

    if(master == NULL)
        pre->next = slave;

    return result;
}

int main()
{
    int a[] = { 1, 5, 8 ,9, 11, 19, 20, 32, 99, 108 };
    int b[] = { 0, 2, 6, 7, 10, 12, 15, 88 };

    Node *na = create_link(a, sizeof(a)/sizeof(a[0]));
    Node *nb = create_link(b, sizeof(b)/sizeof(b[0]));

    process_link(na);
    process_link(nb);

    Node *nc = merge_link(na, nb);

    if(nc)
        process_link(nc);

    return 0;
}
