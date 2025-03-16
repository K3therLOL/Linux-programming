#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char **argv)
{
    if(argc != 4) {
        fprintf(stderr, "Usage: %s <lib> <function> <parameter>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    void *handle = dlopen(argv[1], RTLD_LAZY);
    if(handle == NULL) {
        fprintf(stderr, "Can't find library.\n");
        return EXIT_FAILURE;
    }

    int (*f)(int) = (int (*)(int))dlsym(handle, argv[2]);
    if(f == NULL) {
        fprintf(stderr, "Function not found.\n");
        return EXIT_FAILURE;
    }

    char *end = NULL;
    int param = strtoul(argv[3], &end, 10);
    if(param == 0 || *end != '\0') {
        fprintf(stderr, "Can't perform conversion.\n");
        return EXIT_FAILURE;
    }

    printf("%d\n", f(param));
}
