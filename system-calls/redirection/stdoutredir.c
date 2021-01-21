#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Error during stdout redirection.\nTry %s <filename>\n\n", argv[0]);
        exit(1);
    }
    int fd = open(argv[1], O_RDWR | O_APPEND | O_CREAT, S_IWGRP | S_IRGRP | S_IWUSR | S_IRUSR);
    close(STDOUT_FILENO);
    dup(fd);    
    close(fd);
    printf("Test string\n");
    return 0;
}