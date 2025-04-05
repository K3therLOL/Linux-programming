#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static size_t get_pidmax(void)
{
    FILE *fp = fopen("/proc/sys/kernel/pid_max", "r");
    if(fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE); 
    }

    char *pid_max = NULL;
    fscanf(fp, "%ms", &pid_max);
    
    size_t pid_max_size = strlen(pid_max);

    free(pid_max);
    fclose(fp);
    return pid_max_size;
}

static char *pid2str(pid_t pid)
{
    size_t pid_buf = get_pidmax() + 1;
    char *pid_s = (char *)calloc(pid_buf, sizeof(char));
    if(pid_s == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    snprintf(pid_s, pid_buf, "%d", pid);
    return pid_s;
}

static size_t get_path_size(char **pid_parts)
{
    size_t buf = 0;
    
    char **p;
    for(p = pid_parts; *p != NULL; p++) {
        buf += strlen(*p);
    }

    buf++;
    return buf;
}
static char *create_path(char *path, char **pid_parts) 
{
    char **p;
    for(p = pid_parts; *p != NULL; p++) {
        strcat(path, *p);
    }

    return path;
}
static char *pid2path(pid_t pid)
{ 
    char *pid_s = pid2str(pid);
    
    char *pid_parts[] = { "/proc/", pid_s, "/status", NULL };

    size_t path_size = get_path_size(pid_parts);
    char *path = (char *)calloc(path_size, sizeof(char));
    if(path == NULL) {
        free(pid_s);
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    
    path = create_path(path, pid_parts);
    free(pid_s);
    return path;
}

#define BUF 6
pid_t get_ppid(void)
{
    pid_t pid = getpid();
    printf("%d\n", pid);    
    FILE *fp = fopen(pid2path(pid), "r");
    if(fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    char *ppid_s = (char *)calloc(get_pidmax(), sizeof(char));
    if(ppid_s == NULL) {
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    char word[BUF] = {0};
    while(fscanf(fp, "%5s", word) && strcmp(word, "PPid:"))
        ;
    long offset = ftell(fp);

    fscanf(fp, "%s", ppid_s);
    printf("%s\n", ppid_s);
    pid = atoi(ppid_s);
    fclose(fp);
    do
    {
        fp = fopen(pid2path(pid), "r");
        if(fp == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        fseek(fp, offset, SEEK_CUR);
        fscanf(fp, "%s", ppid_s);
        printf("%s\n", ppid_s);
        pid = atoi(ppid_s);

        fclose(fp);
    }while(pid != 1);

    return pid;
}

int main()
{
    get_ppid();
}
