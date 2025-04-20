#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *create_command(char *prog, char *param)
{
    char *command = (char *)calloc(strlen(prog) + strlen(param) + 4, sizeof(char));
    if (command == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    
    sprintf(command, "./%s %s", prog, param);
    return command;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <prog> <param>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    char *command = create_command(argv[1], argv[2]);
    FILE *fp      = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        return EXIT_FAILURE;
    }
    
    int c, count = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '0') {
            count++;
        }
    }

    printf("%d\n", count);
    pclose(fp);
    free(command);
}
