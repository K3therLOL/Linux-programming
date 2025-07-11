#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t daemon_end = 0;

void handler(int sig)
{
    daemon_end = 1;
}

int main()
{
    printf("%d\n", getpid());

    struct sigaction act;
    act.sa_handler = handler;
    if (sigaction(SIGURG, &act, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    if (daemon(0, 0) == -1) {
        perror("daemon");
        exit(1);
    }
    
    while (!daemon_end) {}
}
