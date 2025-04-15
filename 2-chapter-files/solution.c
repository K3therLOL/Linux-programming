#define _GNU_SOURCE
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

int main(int argc, char **argv)
{
    char         *buf;
    ssize_t      nbytes, bufsiz;
    struct stat  sb;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <symlink> <reference>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    unlink(argv[1]);        

    if (symlink(argv[2], argv[1]) == -1) {
        perror("symlink");
        exit(EXIT_FAILURE);
    }

    if (lstat(argv[1], &sb) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    /* Add one to the link size, so that we can determine whether
       the buffer returned by readlink() was truncated. */

    bufsiz = sb.st_size + 1;

    /* Some magic symlinks under (for example) /proc and /sys
       report 'st_size' as zero. In that case, take PATH_MAX as
       a "good enough" estimate. */

    if (sb.st_size == 0)
        bufsiz = PATH_MAX;
    
    buf = malloc(bufsiz);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    nbytes = readlink(argv[1], buf, bufsiz);
    if (nbytes == -1) {
        perror("readlink");
        exit(EXIT_FAILURE);
    }

    printf("'%s' points to '%.*s'\n", argv[1], (int) nbytes, buf);

    if (nbytes == bufsiz)
        printf("(Returned buffer may have been truncated)\n");

    free(buf);
    exit(EXIT_SUCCESS);
}
