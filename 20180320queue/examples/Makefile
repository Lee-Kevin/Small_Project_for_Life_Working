CC = gcc
STD = -std=c99
CFLAGS = $(STD) -pthread -I ../src

simple.out: simple.c ../src/queue.h 
	$(CC) $(CFLAGS) ../src/queue.c simple.c -g -o simple.out
