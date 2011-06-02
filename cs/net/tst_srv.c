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

#include <poll.h>
#include <limits.h>

#include <arpa/inet.h>

#define MAXLINE 1024
#define OPENMAX 1024

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

void add_client_fd(struct pollfd *clients, int fd, int *pindex)
{
    int i = 0;

    for(; i < *pindex; i++)
    {
        if(clients[i].fd == -1)
        {
            clients[i].fd = fd;
            clients[i].events = POLLIN;
            break;
        }
    }

    if(i == *pindex)
    {
        *pindex += 1;
        clients[*pindex].fd = fd;
        clients[*pindex].events = POLLIN;
    }

    return;
}

int pread2write(int fd)
{
    char buf[MAXLINE];
    memset(buf, '\0', sizeof(buf));

    int len = read(fd, buf, sizeof(buf));
    if(len == 0)
    {
        printf("end of data\n");
        return 1;
    } else if(len > 0)
    {
        write(fd, buf, len);
    } else
    {
        perror("read error");
        return -1;
    }

    return 0;
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr, caddr;
    socklen_t len;
    char *str_caddr = NULL;
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

    struct pollfd clients[OPENMAX];
    int i, index, retval;

    clients[0].fd = fd;
    clients[0].events = POLLIN;
    index = 0;

    /* initial client array */
    for(i = 1; i < OPENMAX; i++)
        clients[i].fd = -1;

    while(1)
    {
        retval = poll(clients, index + 1, -1); /* infinite timeout */

        if(retval > 0)
        {
            if(clients[0].revents & POLLIN) /* accept new connection */
            {
                len = sizeof(caddr);
                cfd = accept(clients[0].fd, (struct sockaddr*)&caddr, &len);

                str_caddr = inet_ntoa(caddr.sin_addr);
                printf("connection from %s, port %d\n", str_caddr, ntohs(caddr.sin_port));
                if(cfd <= 0)
                    perror("accept error");
                else
                    add_client_fd(clients, cfd, &index);
            }

            for(i = 1; i <= index; i++)
            {
                if(clients[i].fd < 0)
                    continue;
                if(clients[i].revents & POLLIN)
                {
                    retval = pread2write(clients[i].fd);
                    if(retval == 1) /* client socket closed */
                    {
                        close(clients[i].fd);
                        clients[i].fd = -1;
                    }
                }
            }
        }
    }

    /*
    while(1)
    {
        len = sizeof(caddr);
        cfd = accept(fd, (struct sockaddr*)&caddr, &len);
        if(cfd < 0)
            perror("accept error");
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
    }
    */

    close(fd);
    exit(0);
}
