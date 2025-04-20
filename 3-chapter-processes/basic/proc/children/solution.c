#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF 10
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
        exit(EXIT_FAILURE);
    }

    sprintf(path, "/proc/%d/task/%d/children", pid, pid);
    return path;
}

void rec_children(pid_t pid, int *cnt)
{
    FILE *fp = NULL; pid_t *children = NULL;
    char *path = get_path(pid);

    fp = fopen(path, "r");
    if(fp == NULL) {
        perror("fopen");
        goto clean;
    }

    size_t size = 0, cap = BUF;
    children = (pid_t *)malloc(cap * sizeof(int));
    if(children == NULL) { 
        perror("malloc");
        goto clean;
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
                goto clean;
            }
        }
    }

    fclose(fp);
    fp = NULL;
    
    size_t i;
    for (i = 0; i < size; ++i) {
        rec_children(children[i], cnt);
    }
    
clean:
    if(fp != ULL) {
        fclose(fp);
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
        return EXIT_FAILURE;
    }
    
    char *endptr = NULL;
    pid_t pid = strtoul(argv[1], &endptr, 10);
    if(pid == 0 || *endptr != '\0') {
        fprintf(stderr, "Invalid pid.\n");
        return EXIT_FAILURE;
    }

    printf("%d\n", children(pid));
    return EXIT_SUCCESS;
}
