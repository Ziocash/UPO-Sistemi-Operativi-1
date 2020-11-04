#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int f,i,n,ne;
	char buf[20];

	f = open("testfile", O_RDONLY);
	if (f == -1) 
	{ 
		perror("Unable to open file: ");
		return 1;
	}
	else
	{
		printf("Reader bytes number (max 20): ");
		scanf("%d",&n);
		if (n>20)
			n=20;
		ne = read(f, buf, n);
		printf("read %d characters: ",ne);
		for (i = 0; i < ne; i++)
			putchar(buf[i]);
		printf("\n");
		return 0; 	
	}
}

