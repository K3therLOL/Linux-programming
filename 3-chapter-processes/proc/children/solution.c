#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t get_pidmax(void)
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

char *get_path(pid_t pid)
{
    size_t buf = strlen("/proc/") + 2 * get_pidmax() + strlen("/task/") + strlen("/children") + 1; 
    char *path = (char *)calloc(buf, sizeof(char));
    if(path == NULL) {
        perror("calloc");
        exit(1);
    }

    sprintf(path, "/proc/%d/task/%d/children", pid, pid);
    return path;
}

#define BUF 10
void rec_children(pid_t pid, int *cnt)
{
    char *path = get_path(pid);
    
    FILE *fp = fopen(path, "r");
    if(fp == NULL) {
        perror("fopen");
        exit(1);
    }

    size_t size = 0, cap = BUF;
    pid_t *children = (pid_t *)malloc(cap * sizeof(int));
    if(children == NULL) { 
        perror("malloc");
        exit(1);
    }

    pid_t child;
    while(fscanf(fp, "%d", &child) > 0) {
        (*cnt)++;
        children[size] = child;
        size++;
        if(size == cap) {
            cap *= 2;
            children = (pid_t *)realloc(children, cap * sizeof(pid_t));
            if(children == NULL) {
                perror("realloc");
                exit(1);
            }
        }
    }

    fclose(fp);

    for (size_t i = 0; i < size; ++i) {
        rec_children(children[i], cnt);
    }
    
    free(children);
    free(path);
}

int children(pid_t pid)
{
    int cnt = 0;
    rec_children(pid, &cnt);
    
    return cnt;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        exit(1);
    }
    
    char *endptr = NULL;
    pid_t pid = strtoul(argv[1], &endptr, 10);
    if(pid == 0 || *endptr != '\0') {
        fprintf(stderr, "Invalid pid.\n");
        exit(1);
    }

    printf("%d\n", children(pid));
}
