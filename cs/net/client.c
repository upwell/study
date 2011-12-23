#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <errno.h>

#include <arpa/inet.h>

#define MAXLINE 1024

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr, caddr;
    socklen_t len;
    char *str_caddr = NULL;
    int reuseaddr = 1;
    pid_t cid;
    int cfd;
    char data[] = "abcd";
    char buff[MAXLINE];

    bzero(&saddr, sizeof(struct sockaddr_in));
    bzero(&saddr, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(3120);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
/*    saddr.sin_addr.s_addr = htonl(INADDR_ANY); */
    setsockopt(fd, IPPROTO_TCP, SO_REUSEADDR, (const void*) &reuseaddr,
            sizeof(int));

    if(0 != bind(fd, (struct sockaddr*)&saddr, sizeof(saddr)))
    {
        perror("bind failed");
        return -1;
    }

    if(0 != connect(fd, (struct sockaddr*)&saddr, sizeof(saddr)))
    {
        perror("connect failed");
        return -1;
    }

    while(1)
    {
        memset(buff, 0, sizeof(buff));
        fgets(buff, MAXLINE-1, stdin);
        write(fd, buff, MAXLINE);
    }

    close(fd);
    exit(0);
}
