#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    
    char *buffer = "Hello, World!\n";
    if (fork() == 0) {
        close(pipefd[1]);
        char sym;
        while(read(pipefd[0], &sym, 1) > 0) {
            printf("%c", sym);
        }
        close(pipefd[0]);
    } else {
        close(pipefd[0]);
        for (ssize_t i = 0; i != strlen(buffer); i += write(pipefd[1], buffer + i, strlen(buffer) - i))
            ;
        close(pipefd[1]);
        wait(NULL);
    }

}
