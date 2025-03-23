#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void get_num(void)
{
    srand(time(NULL));
    printf("%d\n", rand());
}
