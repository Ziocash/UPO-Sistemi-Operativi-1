#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "call.h"

int main(int argc, char *argv[])
{
    int f,i,n,ne;
    char buf[20];

    call( f = open("testfile",O_RDONLY), "Error opening file");
    printf("Reader bytes number (max 20): ");
    scanf("%d",&n);
    call( ne = read(f, buf, n), "Read error");
    printf("Read %d characters: ",ne);
    for (i = 0; i < ne; i++)
        putchar(buf[i]);
    printf("\n");
    return 0;
}

