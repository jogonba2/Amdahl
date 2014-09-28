CC = gcc

amdahl : amdahl.o
		 $(CC) -fopenmp amdahl.c -o amdahl