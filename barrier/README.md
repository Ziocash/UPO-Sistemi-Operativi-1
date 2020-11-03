# Barrier

Request: synchronize an undefined number of processes using semaphores in C

- Synchronization point between `n-1` processes, the last one wakes up the others
- Requested objects:
  - 1 **mutex** to access shared counter
  - 1 **semaphore** to synchronize processes

## Critical section

Accessing the critical section is fundamental, also granting progress:

- `shared->counter` must be accessed "in order" to grant progress and synchronization
- mutex must be released before downing a semaphore (reverting the order results in a deadlock)

```c
void wall(int taskid)
{
    printf("downing\n");
    down(semid, 0);
    //This part has access granted only if the mutex is up (1)
    if(shared->counter < n - 1 )
    {
        shared->counter++;
        printf("Shared counter = %d, incremented by task %d\n", shared->counter, taskid);
        //Releasing the mutex to let others processes continue
        up(semid, 0);
        //Downing sync semaphore to wait others processes
        down(semid, 1);
    }
    else
    {
        up(semid, 1);
        shared->counter = 0;
        up(semid, 0);
    }
}
```

## Synchronization

- `semfun.h` header containing seamphores API (provided by teacher)
- `semfun.c` semaphore implementation (`up` and `down` functions)
