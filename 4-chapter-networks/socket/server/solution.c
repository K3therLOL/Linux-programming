#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <service>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *service = argv[1];
    struct addrinfo hints = { .ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM, .ai_flags = AI_PASSIVE };
    struct addrinfo *res = NULL;
    int errcode;
    if ((errcode = getaddrinfo(NULL, service, &hints, &res))) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(errcode));
        return EXIT_FAILURE;
    }

    int server_fd = -1, connection = -1;
    for (struct addrinfo *addr = res; addr != NULL; addr = addr->ai_next) {
        if ((server_fd = socket(addr->ai_family, addr->ai_socktype, 0)) == -1) {
            perror("socket");
            return EXIT_FAILURE;
        }

        if (bind(server_fd, addr->ai_addr, addr->ai_addrlen) == -1) {
            close(server_fd);
            server_fd = -1;
            continue;
        }

        if (listen(server_fd, SOMAXCONN) == -1) {
            close(server_fd);
            server_fd = -1;
            continue;
        }

        if ((connection = accept(server_fd, NULL, NULL)) == -1) {
            close(server_fd);
            continue;
        } else {
            write(connection, "Hello from server!\n", strlen("Hello from server!\n"));
            close(connection);
        }
    }
    freeaddrinfo(res);

    if (connection == -1) {
        fprintf(stderr, "Can't establish server\n");
        return EXIT_FAILURE;
    }

}

