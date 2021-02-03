
#include <stdio.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "semfun.h"
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

//Mutex and semaphore macros
/*Mutex for pan access*/
#define MUTEX 0
/*Empty pan semaphore*/
#define EMPTY 1
/*Full pan semaphore*/
#define FULL 2

//Global variables
/*Number of consumers*/
int task_number = 0;
/*Shared memory id*/
int sharedid = 0;
/*Semaphore struct ID*/
int semaphoreid;

/*Shared variables structure*/
typedef struct commons
{
    int portions;
    int fill_up_times;
    int number_portions;
    int repetitions;
} vars;

/*Pointer to access shared variables*/
vars *shared;

void cooker();
void eater(int taskid);
void usage(int argc, char *executable);

int main(int argc, char *argv[])
{
    usage(argc, argv[0]);

    semaphoreid = semget(IPC_PRIVATE, 3, 0600);
    if (semaphoreid == -1)
    {
        perror("Error during semaphore creation.\n");
        exit(EXIT_FAILURE);
    }

    //Shared memory creation
    sharedid = shmget(IPC_PRIVATE, sizeof(vars), 0600);
    //Coonect memory segment to a pointer
    shared = (vars *)shmat(sharedid, NULL, 0);

    task_number = atoi(argv[1]);
    shared->portions = shared->number_portions = atoi(argv[2]);
    shared->repetitions = atoi(argv[3]);

    seminit(semaphoreid, MUTEX, 1);
    seminit(semaphoreid, EMPTY, 0);
    seminit(semaphoreid, FULL, 0);

    pid_t cookerpid = fork();
    if (cookerpid == (pid_t)-1)
    {
        perror("Error during cooker fork.\n");
        exit(EXIT_FAILURE);
    }
    else if (cookerpid == (pid_t)0)
        cooker();

    for (int i = 0; i < task_number; i++)
    {
        pid_t eaterpid = fork();
        if (eaterpid == (pid_t)-1)
        {
            perror("Error during cooker fork.\n");
            exit(EXIT_FAILURE);
        }
        else if (eaterpid == (pid_t)0)
            eater(i + 1);
    }

    for (int i = 0; i < task_number; i++)
        wait(NULL);

    kill(cookerpid, SIGTERM);

    shmctl(sharedid, IPC_RMID, NULL);
    printf("The pan was fill up %d times. Remaining portions: %d\n\n", shared->fill_up_times, shared->portions);
}

/*Checks if arguments are 4*/
void usage(int argc, char *executable)
{
    if (argc != 4)
    {
        printf("Missing arguments.\n");
        printf("You should type %s #task_n #portions #repetitions\n", executable);
        exit(EXIT_FAILURE);
    }
}

void cooker()
{
    while (1)
    {
        down(semaphoreid, EMPTY);

        if (shared->portions == 0)
        {
            for (int i = 0; i < shared->number_portions; ++i)
                shared->portions++;

            shared->fill_up_times++;
            printf("\nPending operation complete. Producer is waiting.\n\n");
            up(semaphoreid, FULL);
        }
    }
}

void eater(int taskid)
{
    for (int i = 0; i < shared->repetitions; ++i)
    {
        down(semaphoreid, MUTEX);
        printf("Eater %d shared variables access granted.\n", taskid);
        if (shared->portions == 0)
        {
            printf("Empty pan, calling cooker\n");
            up(semaphoreid, EMPTY);
            down(semaphoreid, FULL);
        }
        shared->portions--;
        printf("Eater %d eats a portion. Remaining portions: %d\n", taskid, shared->portions);
        up(semaphoreid, MUTEX);
        usleep(500);
    }
    exit(EXIT_SUCCESS);
}