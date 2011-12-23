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

#define MAXLINE 1024

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
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* saddr.sin_addr.s_addr = htonl(INADDR_ANY); */

    bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    listen(fd, 5);

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

    close(fd);
    exit(0);
}
