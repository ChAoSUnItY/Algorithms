
CC=gcc
CFLAGS=-Wall -g

%.o: %.c
	$(CC) $(CFLAGS) -c $^

all: myrandom.o mytree.o validate.o
	$(CC) $(CFLAGS) main.c myrandom.o mytree.o validate.o -o main

dbg1: myrandom.o mytree.o validate.o
	$(CC) $(CFLAGS) -DDEBUG1 main.c myrandom.o mytree.o validate.o -o main

clean:
	rm *.o *.so *.a
