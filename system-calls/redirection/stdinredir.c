#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *s = malloc(sizeof(char *));
    if (s == NULL)
    {
        perror("Unable to allocate memory.\n");
        exit(1);
    }
    if (argc == 2)
    {
        int fd = open(argv[1], O_RDWR | O_APPEND | O_CREAT, S_IWGRP | S_IRGRP | S_IWUSR | S_IRUSR);
        if (fd == -1)
        {
            perror("Unable to open file.\n");
            exit(1);
        }
        close(STDIN_FILENO);
        dup(fd);
        close(fd);
    }
    scanf("%s\n", s);
    printf("Printing: %s\n", s);
    return 0;
}