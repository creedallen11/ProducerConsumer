CC=gcc
CFLAGS=-std=c99 -g -Wall -Wpedantic -Werror -pthread
LFLAGS=-pthread -lm


%.o: %.c
	$(CC) $(CFLAGS) -c $? -o $@ 

all: ProducerConsumer

ProducerConsumer: ProducerConsumer.o BoundedBuffer.o Request.o
	$(CC) ProducerConsumer.o BoundedBuffer.o Request.o -o ProducerConsumer $(LFLAGS)

clean:
	rm -f *.o
	rm -f ProducerConsumer