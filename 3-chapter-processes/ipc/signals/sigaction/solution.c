#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t usr1_cnt = 0;
volatile sig_atomic_t usr2_cnt = 0;
volatile sig_atomic_t is_end   = 0;

void usr1_handler(int sig) {
    usr1_cnt++;
}

void usr2_handler(int sig) {
    usr2_cnt++;
}

void term_handler(int sig) {
    is_end = 1;
}

int main()
{
    struct sigaction usr1_act, usr2_act, term_act;
    usr1_act.sa_handler   = usr1_handler;
    usr2_act.sa_handler   = usr2_handler;
    term_act.sa_handler   = term_handler;

    if(sigaction(SIGUSR1, &usr1_act, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    if(sigaction(SIGUSR2, &usr2_act, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    if(sigaction(SIGTERM, &term_act, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while(!is_end) {}
    
    printf("%d %d\n", usr1_cnt, usr2_cnt); 
}
