/* Условие задачи:
 * 
 * В системе существуют 2 региона разделяемой памяти, заполненной некоторыми числами (типа int). 
 * Каждый из регионов имеет размер 1000 байт. Вам требуется разработать приложение, 
 * которое попарно суммирует первые 100 чисел в этих регионах и помещает суммы в новый (созданный вашим приложением) 
 * регион памяти размером 1000 байт.
 * Таким образом, после завершения работы Вашего приложения в памяти должен существовать регион разделяемой памяти размером 
 * 1000 байт, содержащий в начале 100 сумм.
 * Перед завершением работы приложение выводит в стандартный поток ввода-вывода ключ созданного региона, 
 * завершающийся символом конца строки. На вход ваше приложение принимает ключи существующих регионов памяти. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define BUF     1000
#define SIZE    100
#define PROJ_ID 123

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

key_t insert_integers(key_t key, const int *x, const int *y)
{
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
    key_t key = ftok(argv[0], PROJ_ID);     
    printf("%d\n", insert_integers(key, get_integers(atoi(argv[1])), get_integers(atoi(argv[2]))));
    return EXIT_SUCCESS;
}
