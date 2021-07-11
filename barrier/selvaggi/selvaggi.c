
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
    // Portions available in the pan
    int portions;
    // Fill up times
    int fill_up_times;
    // Number of portions passed on launch
    int number_portions;
    // Number of repetions each eater must do before terminate
    int repetitions;
} vars;

/*Pointer to access shared variables*/
vars *shared;

/**
 * @brief Function representing producer task
 * 
 */
void cooker();

/**
 * @brief Function representing consumer tasks
 * 
 * @param taskid Task id
 */
void eater(int taskid);

/**
 * @brief Check if parameters are correctly passed to executable
 * 
 * @param argc Number of arguments
 * @param executable Executable string name
 */
void usage(int argc, char *executable);

/**
 * @brief Code entry point
 * 
 */
int main(int argc, char *argv[])
{
    usage(argc, argv[0]);

    //Semaphore initialization 
    //IPC_PRIVATE
    semaphoreid = semget(IPC_PRIVATE, 3, 0600);
    if (semaphoreid == -1)
    {
        perror("Error during semaphore creation.\n");
        exit(EXIT_FAILURE);
    }

    /*
     * IPC_PRIVATE create a brand new set of semaphores.
     * 
     * 0600 = Owner can read and  write
     */
    sharedid = shmget(IPC_PRIVATE, sizeof(vars), 0600);
    //Cast memory segment to vars pointer
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

    //printf("PID %d terminating\n", wait(NULL));    

    kill(cookerpid, SIGTERM);

    shmctl(sharedid, IPC_RMID, NULL);
    printf("The pan was filled up %d times. Remaining portions: %d\n\n", shared->fill_up_times, shared->portions);
}

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
        //Entering critical section
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
        //Exiting critical section
        up(semaphoreid, MUTEX);
        usleep(500);
    }
    exit(EXIT_SUCCESS);
}