#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10
#define LENGTH 16

typedef struct node {
    int val;
    struct node *next;
}node;

void print_link(node *head)
{
    if(!head)
    {
        printf("empty link\n");
        return;
    }

    printf("Link Content:\n");
    while(head)
    {
        printf("%d ", head->val);
        head = head->next;
    }
    printf("\n");

    return;
}

void reverse_link(node **phead)
{
    if(!phead)
    {
        printf("invalid input\n");
        return;
    }

    node *head = *phead;
    node *new_head = NULL;
    if(!head)
    {
        printf("empty link\n");
        return;
    }

    printf("... reverse link ...\n");

    node *link_index = head;
    node *tmp;
    while(link_index)
    {
        if(!new_head)
        {
            new_head = link_index;
            link_index = link_index->next;
            new_head->next = NULL;
        } else
        {
            tmp = link_index->next;
            link_index->next = new_head;
            new_head = link_index;
            link_index = tmp;
        }
    }

    *phead = new_head;
}

int main()
{
    int i = 0;
    char tmp_num[LENGTH];

    node *head, *tail, *tmp;
    head = tail = NULL;

    printf("Enter %d integers :\n", SIZE);
    while(i < SIZE)
    {
        memset(tmp_num, '\0', LENGTH);
        scanf("%s", tmp_num);

        tmp = (node*) malloc(sizeof(node));
        tmp->val = atoi(tmp_num);
        tmp->next = NULL;

        if(!head)
            head = tail = tmp;
        else
        {
            tail->next = tmp;
            tail = tmp;
        }

        i++;
    }

    print_link(head);
    reverse_link(&head);
    print_link(head);

    return 0;
}
