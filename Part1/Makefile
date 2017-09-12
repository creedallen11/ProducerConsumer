CC=gcc
CFLAGS=-std=c99 -g -Wall -Wpedantic -Werror
LFLAGS=-lm

default: rr fcfs

all: rr fcfs
rr: rr.c
	$(CC) $(CFLAGS) rr.c -o rr $(LFLAGS)
fcfs: fcfs.c
	$(CC) $(CFLAGS) fcfs.c -o fcfs $(LFLAGS)



clean:
	rm -f rr
	rm -f fcfs
