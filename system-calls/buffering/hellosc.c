#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
   write(1, "Hello \n", 7);
   fflush(stdout);
   for (;;);
}
