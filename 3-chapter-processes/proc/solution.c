#include <stdio.h>
#include <unistd.h>

char *pid2path(pid_t pid)
{
    FILE *fp = fopen("/proc/sys/kernel/pid_max", "r");
    if(fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char *pid_max = NULL;
    scanf(fp, "%s", pid_max);
}

long shift(void)
{
    pid_t pid = getpid();
    
    FILE *fp = fopen(pid2path(pid), "r");
    if(fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while(scanf(fp, "%s", word) && strcmp(word, "PPid"))
        ;
}
    
