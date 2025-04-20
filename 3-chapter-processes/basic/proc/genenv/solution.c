#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int filter(const struct dirent *dir)
{
    const char *s;
    for (s = dir->d_name; *s != '\0'; s++) {
        if(isdigit(*s) == 0) {
            return 0;
        }
    }

    return 1;
}

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

int main(void)
{
    struct dirent **namelist;
    int n;

    n = scandir("/proc", &namelist, filter, NULL);
    if (n == -1) {
        perror("scandir");
        return EXIT_FAILURE; 
    }
    
    int cnt = 0;
    while (n--) {
        char *path = (char *)calloc(strlen("/proc/") + get_pidmax() + strlen("/stat") + 1, sizeof(char));
        if(path == NULL) {
            perror("calloc");
            exit(EXIT_FAILURE);
        }

        sprintf(path, "/proc/%s/stat", namelist[n]->d_name);
        FILE *fp = fopen(path, "r");
        if(fp == NULL) {
            free(path);
            continue;
        }
        
        char name[256] = {0};
        fscanf(fp, "%*s (%[^)]", name);
        if(strcmp(name, "genenv") == 0) {
            cnt++;
        }

        fclose(fp);
        free(path);
        free(namelist[n]);
    }

    printf("%d\n", cnt);
    free(namelist);
}
