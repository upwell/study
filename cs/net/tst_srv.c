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
#include <signal.h>

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

void write_cb(struct bufferevent *bev, void *args)
{
    printf("inside writecb\n");
    return;
}

void read_cb(struct bufferevent *bev, void *args)
{
    char *pbuff;
    size_t len;

    struct evbuffer *in_eb = bufferevent_get_input(bev);
    struct evbuffer *out_eb = bufferevent_get_output(bev);

    len = evbuffer_get_length(in_eb);

    pbuff = evbuffer_readln(in_eb, &len, EVBUFFER_EOL_CRLF);
    if(!pbuff || 0 == len)
    {
        bufferevent_free(bev);
    } else
    {
        evbuffer_add(out_eb, pbuff, len);
    }
}

void conn_accept(int fd, short event, void *args)
{
    printf("inside conn accept [%d], event [%d]\n", fd, event);

    struct event_base *base = args;
    struct bufferevent* be;

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

    be = bufferevent_socket_new(base, cfd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(be, read_cb, write_cb, NULL, be);
    bufferevent_enable(be, EV_READ|EV_WRITE);

    /*
    paras_t *para = (paras_t*) malloc(sizeof(paras_t));
    para->args = base;
    struct event *pev = event_new(base, cfd, EV_READ|EV_PERSIST, handle_client, para);
    para->ev = pev;
    event_add(pev, NULL);
    */

    return;
}

void signal_cb(int sig, short events, void *args)
{
    struct event_base *base = args;

    printf("Caught Exit Signal, quit the loop\n");
    event_base_loopexit(base, NULL);
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
    struct event *intsig;

    listen = event_new(base, fd, EV_READ|EV_PERSIST, conn_accept, (void*)base);
    event_add(listen, NULL);

    intsig = evsignal_new(base, SIGINT, signal_cb, base);
    event_add(intsig, NULL);

    event_base_dispatch(base);

    event_free(listen);
    event_free(intsig);

    close(fd);
    exit(0);
}
