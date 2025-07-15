#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#define HOSTLEN 1024
#define SERVLEN 20

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

        char host[HOSTLEN], serv[SERVLEN];
        if((errcode = getnameinfo(info->ai_addr, info->ai_addrlen, host, HOSTLEN,
                                  serv, SERVLEN, NI_NUMERICHOST | NI_NUMERICSERV))) 
        {
            fprintf(stderr, "%s\n", gai_strerror(errcode));
            continue;
        }
        
        struct protoent *prot = getprotobynumber(info->ai_protocol);
        char *protocol = prot->p_name;

        printf("host: %s\n", host);
        printf("port: %s\n", serv);
        printf("protocol: %s\n", protocol);
        printf("\n");
    }

    freeaddrinfo(res);
}
