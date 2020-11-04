#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{  
    int n = creat("pippo", S_IRUSR | S_IWUSR); 
    close(1);
    dup(n);
    close(n);
    printf("Hello \n");
    return 0;
}
