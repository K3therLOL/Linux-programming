
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <node> <service>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    char *node = argv[1], *service = argv[2];
    struct addrinfo hints = { .ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM, .ai_protocol = 0 };
    struct addrinfo *res = NULL;
    int errcode;
    if ((errcode = getaddrinfo(node, service, &hints, &res))) {
        fprintf(stderr, "%s\n", gai_strerror(errcode));
        return EXIT_FAILURE;
    }

    int fd;
    for (struct addrinfo *info = res; info != NULL; info = info->ai_next) {
        fd = socket(info->ai_family, info->ai_socktype, 0);
        if (fd == -1) {
            perror("socket");
            continue;
        }
        
        if (connect(fd, info->ai_addr, info->ai_addrlen) == -1) {
            close(fd);
            fd = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(res);

    if (fd == -1) {
        fprintf(stderr, "No connection to server.\n");
        return EXIT_FAILURE;
    }

    FILE *fp = fdopen(fd, "w+");
    if (fp == NULL) {
        perror("fdopen");
        return EXIT_FAILURE;
    }

    int bullets = 1000;
    for (int i = bullets; i > 0; --i) {
        fprintf(fp, "Shooting %d bullet\n", i);
    }

    fclose(fp);
}
