
CC=gcc
CFLAGS=-Wall -g

%.o: %.c
	$(CC) $(CFLAGS) -c $^

all: myrandom.o mytree.o
	$(CC) $(CFLAGS) main.c myrandom.o mytree.o -o main

clean:
	rm *.o *.so *.a
