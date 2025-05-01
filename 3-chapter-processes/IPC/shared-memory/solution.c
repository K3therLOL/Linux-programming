#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#define BUF  1000
#define SIZE 100

int *get_integers(key_t key)
{
    int shmid = shmget(key, BUF, 0);
    if(shmid == -1) {
        perror("shmid");
        exit(1);
    }

    int *shmaddr = (int *)shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1) {
        perror("shmaddr");
        exit(1);
    }

    return shmaddr;
}

key_t insert_integers(const int *x, const int *y)
{
    key_t key = rand();
    int shmid = shmget(key, BUF, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmid");
        exit(1);
    }
    
    int *shmaddr = (int *)shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1) {
        perror("shmaddr");
        exit(1);
    }

    int i;
    for (i = 0; i < SIZE; ++i) {
        shmaddr[i] = x[i] + y[i];
    }

    shmdt(x);
    shmdt(y);
    shmdt(shmaddr);
    return key;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <key_1> <key_2>\n", argv[0]);
        return EXIT_FAILURE;
    }
     
    printf("%d\n", insert_integers(get_integers(atoi(argv[1])), get_integers(atoi(argv[2]))));
    return EXIT_SUCCESS;
}
