/*
 * =====================================================================================
 *
 *       Filename:  tst_srv.c
 *
 *    Description:  test net server
 *
 *        Version:  1.0
 *        Created:  06/01/2011 03:55:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Han He (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#include <arpa/inet.h>

#include <event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#define MAXLINE 1024

typedef struct paras
{
    struct event *ev;
    void *args;
} paras_t;

void handle_client(int fd, short event, void* args)
{
    char buf[MAXLINE];
    paras_t *para = (paras_t*) args;
    memset(buf, '\0', sizeof(buf));
    int len;

    if(event & EV_READ)
    {
        len = read(fd, buf, sizeof(buf));
        if(len > 0)
        {
            write(fd, buf, len);
        } else if(len == 0)
        {
            printf("end of data\n");
            close(fd);
            event_del(para->ev); /* this is a must */
            event_free(para->ev);
            free(para);
        }
    }
}

void conn_accept(int fd, short event, void *args)
{
    printf("inside conn accept [%d], event [%d]\n", fd, event);

    struct event_base *base = args;

    struct sockaddr_in caddr;
    socklen_t len;
    char *str_caddr = NULL;
    int cfd;

    len = sizeof(caddr);
    cfd = accept(fd, (struct sockaddr*) &caddr, &len);

    str_caddr = inet_ntoa(caddr.sin_addr);
    if(!str_caddr)
    {
        perror("inet_ntoa failed\n");
    }
    printf("connection from %s, port %d\n", str_caddr, ntohs(caddr.sin_port));

    /* add the client fd into the interested list */
    paras_t *para = (paras_t*) malloc(sizeof(paras_t));
    para->args = base;
    struct event *pev = event_new(base, cfd, EV_READ|EV_PERSIST, handle_client, para);
    para->ev = pev;
    event_add(pev, NULL);

    return;
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    pid_t cid;
    int cfd;
    char data[] = "abcd";
    bzero(&saddr, sizeof(struct sockaddr_in));
    bzero(&saddr, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(3120);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    listen(fd, 5);

    struct event_base *base = event_base_new();
    struct event *listen;

    listen = event_new(base, fd, EV_READ|EV_PERSIST, conn_accept, (void*)base);
    event_add(listen, NULL);
    event_base_dispatch(base);

    close(fd);
    exit(0);
}
