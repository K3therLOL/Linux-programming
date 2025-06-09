/* Задание:
* В текущем каталоге есть 2 канала in1 in2, в которые в случайном порядке поступают числа, которые необходимо просуммировать и вывести окончательную сумму на экран. Сумма выводится в отдельной строке, завершающейся символом конца строки. Признаком окончания подачи символов в каналы является закрытие этих каналов посылающей стороной. */
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>

int file_sum(int fd)
{

    int cap = fcntl(fd, F_GETPIPE_SZ);

    char *content = (char *)malloc(cap + 1);
    if (content == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    ssize_t size = read(fd, content, cap);
    if (size == -1) {
        free(content);
        perror("size");
        exit(EXIT_FAILURE);
    } else if(size == 0) {
        fd = -1;
        return 0;
    }

    content[size] = '\0';
    char *delim = " \t\n";
    int sum = 0;
    for (char *token = strtok(content, delim); token != NULL; token = strtok(NULL, delim)) {
        int num;
        if ((num = atoi(token)) != 0) {
            sum += num;
        }
    }
    
    printf("file_sum = %d\n", sum);
    free(content);
    return sum;
}

void initialize_set(fd_set *set, int f1, int f2)
{
    FD_ZERO(set);
    if (f1 != -1) {
        FD_SET(f1, set);
    }

    if (f2 != -1) {
        FD_SET(f2, set);
    }
}

int main()
{
    int f1 = open("./in1", O_RDWR);
    int f2 = open("./in2", O_RDWR);
    if (f1 == -1 || f2 == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    fd_set read_set;
    int sum = 0;
    while ((f1 = open("./in1", O_RDWR)) != -1 || (f2 = open("./in2", O_RDWR)) != -1)
    {
        int max_fd = f1 > f2 ? f1 : f2;
        initialize_set(&read_set, f1, f2);
        if(select(max_fd + 1, &read_set, NULL, NULL, NULL) == -1) {
            perror("select");
            return EXIT_FAILURE;
        }

        if (f1 != -1 && FD_ISSET(f1, &read_set)) {
            sum += file_sum(f1);
        }

        if (f2 != -1 && FD_ISSET(f2, &read_set)) {
            sum += file_sum(f2);
        }
    }

    printf("%d\n", sum);
}
