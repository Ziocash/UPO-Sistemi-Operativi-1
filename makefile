barrier: barrier.o semfun.o
	gcc -o barrier -Wall -pedantic semfun.o barrier.o 
barrier.o: barrier.c
	gcc -c -Wall -pedantic barrier.c
semfun.o: semfun.c semfun.h
	gcc -c -Wall -pedantic semfun.c
clean:
	rm barrier semfun.o barrier.o