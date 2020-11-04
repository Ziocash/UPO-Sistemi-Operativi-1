#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    //Checking if the user inserts too few parameters
    if(argc < 3)
    {
        write(STDOUT_FILENO, "Missing parameters.\nTry using ./append <filename> <\"string\">\n", 61);
        exit(1);
    }
    
    //Checking if the user inserts too many parameters
    if(argc > 3)
    {
        write(STDOUT_FILENO, "Too many parameters.\nTry using ./append <filename> <\"string\">\n", 64);
        exit(1);
    }

    int fd = open(argv[1], O_RDWR | O_APPEND | O_CREAT, S_IWGRP | S_IRGRP | S_IWUSR | S_IRUSR);
    write(fd, argv[2], strlen(argv[2]));
    close(fd);
    write(STDOUT_FILENO, "File created correctly, content loaded.\nTerminating...\n\n", 58);

    return 0;
}