#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error, uncorrect usage.\nTry %s <file1> <file2> instead.\n", argv[0]);
        exit(1);
    }
    int pipefd[3];
    pipe(pipefd);
    if (fork() == (pid_t)0)
    {
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("cat", "cat", argv[2], NULL);
        perror("exec cat failed");
    }
    else if (fork() == (pid_t)0)
    {
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("cat", "cat", argv[1], NULL);
        perror("exec cat failed");
    }
    else
    {
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("more", "more", pipefd[1], NULL);
        wait(NULL);
        wait(NULL);
    }
    return 0;
}
