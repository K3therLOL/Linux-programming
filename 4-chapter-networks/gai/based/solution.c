#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#define BUFFER 1024

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <node> <service>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    char *node = argv[1], *service = argv[2];

    struct addrinfo *res = NULL;
    int errcode;
    if ((errcode = getaddrinfo(node, service, NULL, &res))) {
        fprintf(stderr, "%s\n", gai_strerror(errcode));
        return EXIT_FAILURE;
    }

    for (struct addrinfo *info = res; info != NULL; info = info->ai_next) {
        char ip[BUFFER] = {0};
        
        in_port_t port = 0;
        if (info->ai_family == AF_INET) {
            struct sockaddr_in *ipv4_addr = (struct sockaddr_in *)info->ai_addr;
            inet_ntop(info->ai_family, &ipv4_addr->sin_addr, ip, info->ai_addrlen);
            port = ntohs(ipv4_addr->sin_port);
        } else if (info->ai_family == AF_INET6)  {
            struct sockaddr_in6 *ipv6_addr = (struct sockaddr_in6 *)info->ai_addr;
            inet_ntop(info->ai_family, &ipv6_addr->sin6_addr, ip, info->ai_addrlen);
            port = ntohs(ipv6_addr->sin6_port);
        }

        printf("ip: %s\n", ip);
        printf("port: %d\n", port);
        printf("\n");
    }

    freeaddrinfo(res);
}
