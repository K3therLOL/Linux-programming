#include <stdio.h>
#include <sys/types.h>                                       
#include <sys/stat.h> 
#include <fcntl.h>

int main()
{
    mkfifo("fifo", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
}
