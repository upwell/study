#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    in_addr_t inaddr = inet_network("192.168.75.0");
    in_addr_t inaddr_msk = inet_network("255.255.254.0");

    in_addr_t inaddr_dst = inet_network("192.168.74.1");

    if ((inaddr & inaddr_msk) == (inaddr_dst & inaddr_msk))
        printf("same sub net\n");
    else
        printf("not in same sub net\n");

    return 0;
}
