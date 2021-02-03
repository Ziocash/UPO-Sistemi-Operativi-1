#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    pipe(pipefd);
    if (fork() == (pid_t)0)
    {
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("tail", "tail", "-10", NULL);
        perror("exec wc failed");
    }
    else if (fork() == (pid_t)0)
    {
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("head", "head", "-20", "pipes.c", NULL);
        perror("exec ls failed");
    }
    else
    {
        close(pipefd[0]);
        close(pipefd[1]);
        wait(NULL);
        wait(NULL);
    }
    return 0;
}
