mymat: inputHandlers.o mat.o mymat.o
	gcc -g -Wall -ansi -pedantic inputHandlers.o mat.o mymat.o -o mymat

mymat.o: mymat.c inputHandlers.h mat.h
	gcc -c -Wall -ansi -pedantic mymat.c -o mymat.o

mat.o: mat.c  mat.h
	gcc -c -Wall -ansi -pedantic mat.c -o mat.o

inputHandlers.o: inputHandlers.c inputHandlers.h
	gcc -c -Wall -ansi -pedantic inputHandlers.c -o inputHandlers.o
