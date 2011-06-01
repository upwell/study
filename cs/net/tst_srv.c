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
#include <fcntl.h>

#include <arpa/inet.h>
#include <sys/select.h>

#define MAXLINE 1024
#define MAXFDS 1024

void read2write(int fd)
{
    char buf[MAXLINE];
    memset(buf, '\0', sizeof(buf));
    int len;
    while((len = read(fd, buf, sizeof(buf))) > 0)
    {
        write(fd, buf, len);
        memset(buf, '\0', sizeof(buf));
    }

    return;
}

void sread2write(int fd)
{
    char buf[MAXLINE];
    memset(buf, '\0', sizeof(buf));

    int len = read(fd, buf, sizeof(buf));
    if(len > 0)
        write(fd, buf, len);
    else if(len == 0)
        printf("end of data\n");
    else
        perror("error in read");

    return;
}

void setnonblocking(int sock)
{
    int opts;

    opts = fcntl(sock,F_GETFL);
    if (opts < 0) {
        perror("fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock,F_SETFL,opts) < 0) {
        perror("fcntl(F_SETFL)");
        exit(EXIT_FAILURE);
    }
    return;
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    //setnonblocking(fd);
    struct sockaddr_in saddr, caddr;
    socklen_t len;
    char *str_caddr = NULL;
    pid_t cid;
    int cfd;
    int count = 0;
    int retval = 0;
    int maxfd = 0;

    fd_set readset, readyset;
    int fds[MAXFDS];

    FD_ZERO(&readset);
    FD_ZERO(&readyset);

    bzero(&saddr, sizeof(struct sockaddr_in));
    bzero(&caddr, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(3120);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    listen(fd, 5);

    FD_SET(fd, &readset);
    maxfd = fd;

    while(1)
    {
        readyset = readset;
        /* the first para must be the largest fd */
        retval = select(maxfd+1, &readyset, NULL, NULL, NULL);

        if(retval < 0)
        {
            perror("select error\n");
            exit(-1);
        }

        if(FD_ISSET(fd, &readyset))
        {
            printf("in accept\n");
            bzero(&caddr, sizeof(struct sockaddr_in));
            len = sizeof(caddr);

            cfd = accept(fd, (struct sockaddr*)&caddr, &len);
            if(cfd < 0)
                perror("accept error");

            str_caddr = inet_ntoa(caddr.sin_addr);
            printf("connection from %s, port %d\n", str_caddr, ntohs(caddr.sin_port));

            FD_SET(cfd, &readset);
            fds[count] = cfd;
            count++;

            if(cfd > maxfd) maxfd = cfd;
        }

        int i = 0;
        for(; i < count; i++)
        {
            if(FD_ISSET(fds[i], &readyset))
            {
               sread2write(fds[i]);
            }
        }


        /*
        if( (cid = fork()) > 0 )
        {
            continue;
        } else
        {
            close(fd);

            str_caddr = inet_ntoa(caddr.sin_addr);
            if(!str_caddr)
            {
                perror("inet_ntoa failed\n");
            }
            printf("connection from %s, port %d\n", str_caddr, ntohs(caddr.sin_port));
            read2write(cfd);
            close(cfd);
            exit(0);
        }
        */
    }

    close(fd);
    exit(0);
}
