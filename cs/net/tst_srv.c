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

#include <ev.h>

#define MAXLINE 1024

static void handle_client(struct ev_loop *loop, ev_io *w, int revents)
{
    char buf[MAXLINE];
    memset(buf, '\0', sizeof(buf));
    int len;

    if(revents & EV_READ)
    {
        len = read(w->fd, buf, sizeof(buf));
        if(len > 0)
        {
            write(w->fd, buf, len);
        } else if(len == 0)
        {
            printf("end of data\n");
            close(w->fd);
            ev_io_stop(loop, w);
            free(w);
        }
    }
}

static void conn_accept(struct ev_loop *loop, ev_io *w, int revents)
{
    struct sockaddr_in caddr;
    socklen_t len;
    char *str_caddr = NULL;
    int cfd;

    len = sizeof(caddr);
    cfd = accept(w->fd, (struct sockaddr*) &caddr, &len);

    str_caddr = inet_ntoa(caddr.sin_addr);
    if(!str_caddr)
    {
        perror("inet_ntoa failed\n");
    }
    printf("connection from %s, port %d\n", str_caddr, ntohs(caddr.sin_port));

    /* add the client fd into the interested list */
    ev_io *pev = (ev_io*) malloc(sizeof(ev_io));
    ev_io_init(pev, handle_client, cfd, EV_READ);
    ev_io_start(loop, pev);

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

    struct ev_loop *loop = ev_default_loop(0);
    ev_io listener;
    ev_io_init(&listener, conn_accept, fd, EV_READ);
    ev_io_start(loop, &listener);

    ev_run(loop, 0);

    close(fd);
    exit(0);
}
