#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

int *create_shm(void)
{
    int shmid = shmget(IPC_PRIVATE, 1000, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    
    int *shmaddr = (int *)shmat(shmid, NULL, 0);
    for (int i = 0; i < 100; ++i) {
        shmaddr[i] = i;
    }

    return shmaddr;
}

int main()
{
    int *arr1 = create_shm();
    int *arr2 = create_shm();

    for (int i = 0; i < 100; ++i) {
        printf("%d + %d = %d\n", arr1[i], arr2[i], arr1[i] + arr2[i]);
    }
}
