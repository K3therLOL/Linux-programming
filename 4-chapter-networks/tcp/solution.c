#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#define LOCAL_ADDR "127.0.0.1"
#define TERMINAL       "OFF\n" 

int cmp(const void *x, const void *y)
{
    char arg1 = *(char *)x;
    char arg2 = *(char *)y;

    return (arg1 < arg2) - (arg1 > arg2);
}

size_t get_recvbuf(int fd)
{
    size_t buf = 0;
    socklen_t len = sizeof(buf);
    getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &buf, &len);
    return buf;
}
int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <serv>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int service = atoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(service);
    inet_aton(LOCAL_ADDR, &(addr.sin_addr));
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }

    if (listen(sockfd, SOMAXCONN) == -1) {
        perror("listen");
        return EXIT_FAILURE;
    }

    socklen_t address_len = sizeof(addr);
    int fd = accept(sockfd, (struct sockaddr *)&addr, &address_len);
    if (fd == -1) {
        perror("accept");
        return EXIT_FAILURE;
    }

    
    int    read_bytes; 
    size_t recv_buf = get_recvbuf(fd);
    char  *recv_msg = (char *)calloc(recv_buf + 1, sizeof(char));
    if (recv_msg == NULL) {
        perror("calloc");
        return EXIT_FAILURE;
    }

    while((read_bytes = read(fd, recv_msg, recv_buf)) && strncmp(recv_msg, TERMINAL, read_bytes)) {
        char *send_msg = recv_msg;
        qsort(send_msg, read_bytes, sizeof(char), cmp); 
        send_msg[read_bytes] = '\0';
        write(fd, send_msg, read_bytes + 1);
    }
}
