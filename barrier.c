#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <signal.h>
#include "semfun.h"

int shmid = 0;
pid_t semid = 0;
int n = 0;

typedef struct condivise
{
    int counter;
} var_condivise;

var_condivise *shared;

void wall(int taskid)
{
    printf("downing\n");
    down(semid, 0);
    if(shared->counter < n - 1 )
    {
        shared->counter++;
        printf("Shared counter = %d, incremented by task %d\n", shared->counter, taskid);
        up(semid, 0);
        down(semid, 1);
    }
    else
    {
        for (int i = 0; i < n-1; i++)
        {
            up(semid, 1);
        }
        shared->counter = 0;
        up(semid, 0);
    }
    
}

void task(int taskid)
{
    printf("task %d starting...\n", taskid);
    wall(taskid);
    printf("task %d exiting...\n", taskid);
    exit(0);
}

int main(void)
{
    semid = semget(IPC_PRIVATE, 2, 0600);
    if(semid == -1)
    {
        perror("Semget error");
        exit(1);
    }

    shmid = shmget(IPC_PRIVATE, sizeof(var_condivise), 0600);
    if(shmid < 0)
        perror("An error occured creating shared memory space");
    shared = (var_condivise *)shmat(shmid, NULL, 0);

    seminit(semid, 0, 1); /*Mutex accesso al counter*/
    seminit(semid, 1, 0); /*Semaforo di sincronizzazione*/

    printf("Inserire un numero di processi simultanei da eseguire: ");
    scanf(" %d", &n);

    for (int i = 0; i < n; i++)
    {
        pid_t taskid = fork();
        if (taskid == (pid_t)-1)
        {
            perror("An error occurred during the fork");
            return 1;
        }
        else if (taskid == (pid_t)0)
            task(i + 1);
    }

    for (int i=0; i < n; i++)
        wait(NULL);

    printf("Counter set to %d", shared->counter);
    shmctl(shmid, IPC_RMID, NULL);
}