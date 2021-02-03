catmore: catmore.o
	gcc -o catmore -Wall -pedantic catmore.o
catmore.o: catmore.c
	gcc -c -Wall -pedantic append.c
sig: sig.o
	gcc -o sig -Wall -pedantic sig.o
sig.o: sig.c
	gcc -c -Wall -pedantic sig.c
clean:
	rm sig sig.o pipes pipes.o catmore catmore.o