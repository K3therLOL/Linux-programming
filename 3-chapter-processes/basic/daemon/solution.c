#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid = fork();
    if (pid == 0) {
        chdir("/");
        printf("%d\n", setsid());
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
        sleep(1000);
    } 

    return 0;
}
