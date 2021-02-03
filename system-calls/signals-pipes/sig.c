#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void f_intr(int sig)
{
    printf("SIG received\nSIGnum: %d\n", sig);
}

int main(int argc, char *argv[])
{
    int m, n;
    struct sigaction sa;

    printf("PID: %d\n", getpid());

    sa.sa_handler = f_intr;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
    if (fork() == 0)
    {
        execl(argv[0], "");
        exit(0);
    }
    else
    {
        for (m = 0; m < 100; m++)
        {
            for (n = 0; n < 200000000; n++);
            printf("Working %d\n", m);
        }
    }
    printf("PID %d terminated\n", wait(NULL));
    return 0;
}
